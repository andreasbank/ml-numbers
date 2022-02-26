#include <iostream>
#include <fstream>

#include "Brain.hpp"
#include "MyException.hpp"

/** Black and white 8x8 pixels bitmap representing a "1". */
static float img[] = {
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0.1, 0.9, 1, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0,
	0, 0, 0.1, 0.9, 0.9, 0.1, 0, 0
};
static size_t img_len = sizeof (img) / sizeof (img[0]);

void genTrainData(float buf[], size_t& len) {
	float f = 0.0f;
	for (int i = 0; i < len; i++, f += 0.0001) {
		buf[i] = f;
	}
}

void load_file(const std::string file, float buf[], const size_t& buf_len) {
	std::ifstream f(file);
	if (f.is_open()) {
		std::string line;
		size_t i;
		for (i = 0; getline(f, line); i++) {
			buf[i] = std::stof(line, nullptr);
		}
		f.close();
		if (buf_len != i)
			std::cerr << "Invalid data file length" << std::endl;
	} else {
		throw MyException("Failed to open file");
	}
}

void save_file(const std::string file, float buf[], const size_t& buf_len) {
	std::ofstream f(file);
	if (!f.is_open())
		std::cerr << "Failed to open trained data output file." <<
			  std::endl;
	for (int i = 0; i < buf_len; i++)
		f << std::to_string(buf[i]) << std::endl;
	f.close();
}

// TODO: catch all exceptions
int main(int argc, char* argv[]) {
	size_t layers_len = 1;
	size_t outputs_len = 10;
	size_t weights_data_len = img_len + img_len * img_len * layers_len +
				  img_len * outputs_len;
	size_t biases_data_len = img_len + img_len * layers_len + outputs_len;
	float weights_data[weights_data_len] = {0};
	float biases_data[biases_data_len] = {0};

	try {
		load_file("ml-numbers.weights", weights_data, weights_data_len);
		load_file("ml-numbers.biases", biases_data, biases_data_len);
		std::cout << "Loaded train data from file" << std::endl;
	} catch (MyException& e) {
		/* For debugging only */
		genTrainData(weights_data, weights_data_len);
		genTrainData(biases_data, biases_data_len);
		std::cout << "Train data has been generated" << std::endl;
	}

	std::cout << "Image length: " << std::to_string(img_len) <<
		  ", weights length: " << std::to_string(weights_data_len) <<
		  ", biases length: " <<
		  std::to_string(biases_data_len) << std::endl;

	Brain brain(layers_len,
		    weights_data,
		    weights_data_len,
		    biases_data,
		    biases_data_len,
		    1,
		    img,
		    img_len,
		    outputs_len);

	std::unique_ptr<float[]> results = brain.think();

	for (int i = 0; i < outputs_len; i++) {
		std::cout << std::to_string(i) << ": " <<
			  std::to_string(results[i]);
		if (i + 1 < outputs_len)
			std::cout << ", ";
	}
	std::cout << std::endl;

	brain.getTrainedData(weights_data, weights_data_len, biases_data,
			     biases_data_len);

	try {
		save_file("ml-numbers.weights", weights_data, weights_data_len);
		save_file("ml-numbers.biases", biases_data, biases_data_len);
	} catch (MyException& e) {
		std::cerr << "Failed to save file: " << e.what() << std::endl;
	}


	return EXIT_SUCCESS;
}

#include <memory>

#include "Brain.hpp"
#include "MyException.hpp"
#include "NeuronLayer.hpp"

Brain::Brain(const size_t& layers_len,
	     const float weights_data[],
	     const size_t& weights_data_len,
	     const float biases_data[],
	     const size_t& biases_data_len,
	     const size_t& train_samples,
	     const float input[],
	     const size_t& input_len,
	     const size_t& outputs_len):
	     m_layers_len(layers_len),
	     m_inputs_len(input_len),
	     m_weights_data_len(weights_data_len),
	     m_biases_data_len(biases_data_len),
	     m_outputs_len(outputs_len) {

	if (weights_data_len < input_len * m_layers_len + outputs_len)
		throw MyException("Invalid weights trained data length supplied");

	/* Allocate all layers */
	m_layers.reset(new NeuronLayer[m_layers_len]);

	/* Configure the first layer with only one input per Neuron.
		 This is where we feed the image into the neural net. */
	m_layers[0].configure(input_len, input_len, true);

	/* Load the trained data into the layer. */
	m_layers[0].setTrainedData(weights_data, biases_data);

	/* Configure the rest of the layers with full number of inputs.
		 These layers only take input from previous layers. */
	for (int i = 1; i < m_layers_len; i++)
		m_layers[i].configure(input_len, input_len);

	/* Load the trained data into the layers neurons */
	for (int i = 0; i < m_layers_len; i++) {
		/* We need to move the trained data "input_len" elements forward
		   to skip the first layers weights_data and biases_data section
		   and then for every other layer we move it
		   "input_len * input_len * i", since each layer has
		   "input_len * input_len" number of input/weight pairs */
		size_t data_idx = input_len + input_len * input_len * i;
		m_layers[i].setTrainedData(&weights_data[data_idx],
					   &biases_data[data_idx]);
	}

	/* Create the last layer that generates the answers */
	m_results.configure(input_len, outputs_len);
	size_t data_idx = input_len + input_len * input_len * m_layers_len;
	m_results.setTrainedData(&weights_data[data_idx],
				 &biases_data[data_idx]);

	/* Load the image data into the first layer */
	for (size_t i = 0; i < input_len; i++)
		m_layers[0].receiveInput(input[i], i);
}

std::unique_ptr<float[]> Brain::think(void) {
	/* Run all layers excluding the last one, sending each ones output to
	   the next layer */
	for (int i = 0; i < m_layers_len - 1; i++)
		m_layers[i].think(&m_layers[i + 1]);

	/* Run the last layer, sending its output to the results layer */
	m_layers[m_layers_len - 1].think(&m_results);

	/* Run the results layer, and don't send the output anywhere */
	m_results.think();

	return m_results.getResults();
}

void Brain::learn(float answers[]) {
	// TODO: loop layers backwards
	for (int i = m_layers_len - 1; i >= 0; i--) {
		INeuronDataReceiver *l = i > 0 ?
				static_cast<INeuronDataReceiver *>(
					&m_layers[i - 1]) : nullptr;
		m_layers[i].learn(l, answers);
	}
}

void Brain::getTrainedData(float weights[],
			   const size_t& weights_len,
			   float biases[],
			   const size_t& biases_len) const {
	size_t weights_left = weights_len;
	size_t biases_left = biases_len;
	for (int i = 0; i < m_layers_len; i++) {
		m_layers[i].getTrainedData(&weights[weights_len - weights_left],
					   weights_left,
					   &biases[biases_len - biases_left],
					   biases_left);
	}
}

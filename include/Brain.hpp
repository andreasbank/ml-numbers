#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <memory>

#include "NeuronLayer.hpp"

class Brain {
public:
	Brain(const size_t& layers_len,
	      const float weights_data[],
	      const size_t& weights_data_len,
	      const float biases_data[],
	      const size_t& biases_data_len,
	      const size_t& train_samples,
	      const float input[],
	      const size_t& input_len,
	      const size_t& outputs_len);

	std::unique_ptr<float[]> think(void);
	void learn(float answers[]);
	void getTrainedData(float weights[],
	const size_t& weights_len,
	float biases[],
	const size_t& biases_len) const;

private:
	size_t m_layers_len;
	size_t m_inputs_len;
	size_t m_weights_data_len;
	size_t m_biases_data_len;
	size_t m_train_samples;
	const size_t& m_outputs_len;
	std::unique_ptr<NeuronLayer[]> m_layers;
	NeuronLayer m_results;
};

#endif /* BRAIN_HPP */

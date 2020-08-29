#include <iostream>
#include <memory>

#include "NeuronLayer.hpp"
#include "Neuron.hpp"

unsigned int NeuronLayer::c_count = 0;

NeuronLayer::NeuronLayer() {
	m_id = c_count++;
}

void NeuronLayer::configure(const size_t& inputs_len,
			    const size_t& neurons_len,
			    const bool& first_layer) {
	m_inputs.reset(new float[inputs_len]());
	m_inputs_len = inputs_len;
	m_neurons.reset(new Neuron[neurons_len]);
	m_neurons_len = neurons_len;
	for (int i = 0; i < m_neurons_len; i++)
		m_neurons[i].configInputs(m_inputs, inputs_len, first_layer);
}

void NeuronLayer::setTrainedData(const float weights_data[],
				 const float biases_data[]) {
	for (int i = 0; i < m_neurons_len; i++) {
		size_t data_idx = m_inputs_len * i;
		m_neurons[i].setTrainedData(&weights_data[data_idx],
					    biases_data[data_idx]);
	}
}

void NeuronLayer::receiveInput(const float& input, const size_t& idx) {
	m_inputs[idx] = input;
}

void NeuronLayer::think(NeuronLayer *next_layer) {
	for (int i = 0; i < m_neurons_len; i++)
		m_neurons[i].think(
			static_cast<INeuronDataReceiver *>(next_layer), i);
}

std::unique_ptr<float[]> NeuronLayer::getResults(void) const {
	std::unique_ptr<float[]> results(new float[m_neurons_len]);
	for (int i = 0; i < m_neurons_len; i++)
		results[i] = m_neurons[i].getActivationValue();

	return results;
}

void NeuronLayer::getTrainedData(float weights[],
				 size_t& weights_left,
				 float biases[],
				 size_t& biases_left) const {
	size_t weights_len = weights_left;
	for (int i = 0; i < m_neurons_len; i++) {
		m_neurons[i].getTrainedData(
			&weights[weights_len - weights_left],
			weights_left,
			biases[i]);
	}
	biases_left -= m_neurons_len;
}

void NeuronLayer::learn(INeuronDataReceiver *receiver, const float answers[]) {
	// TODO: code!
 (void) receiver;
}

void NeuronLayer::receiveFeedback(const float answers[]) {
	// TODO: code!
}

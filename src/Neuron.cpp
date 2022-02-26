#include <iostream>
#include <string>
#include <cmath>

#include "Math.hpp"
#include "MyException.hpp"
#include "Neuron.hpp"

unsigned int Neuron::c_count = 0;

Neuron::Neuron() {
	m_id = c_count++;
}

unsigned int Neuron::getCount(void) {
	return Neuron::c_count;
}

void Neuron::configInputs(std::shared_ptr<float[]>& inputs,
			  const size_t& inputs_len,
			  const bool& first_layer) {
	m_inputs = inputs;
	m_inputs_len = inputs_len;
	m_weights_len = first_layer ? 1 : inputs_len;
	m_weights.reset(new float[m_weights_len]());
}

void Neuron::setTrainedData(const float weights_data[],
			    const float& bias_data) {
	std::cout << "Neuron " << std::to_string(m_id) << " (we/bi " <<
			m_weights_len << "): ";
	for (int i = 0; i < m_weights_len; i++) {
		m_weights[i] = weights_data[i];
	}
	m_bias = bias_data;
	std::cout << "w: " << std::to_string(m_weights[0]) << ".." <<
		  std::to_string(m_weights[m_weights_len - 1]) << ", b: " <<
		  std::to_string(m_bias) << std::endl;
}

void Neuron::getTrainedData(float weights[],
			    size_t& weights_len,
			    float& bias) const {
	if (weights_len < m_weights_len)
		throw MyException(
			"Buffer not large enough to return trained data");

	for (int i = 0; i < m_weights_len; i++) {
		weights[i] = m_weights[i];
	}
	bias = m_bias;

	weights_len -= m_weights_len;
}

void Neuron::think(INeuronDataReceiver *receiver, const size_t& idx) {
	for (int i = 0; i < m_weights_len; i++)
		m_activation += m_inputs[i] * m_weights[i];

	m_activation = Math::fast_sigmoid(m_activation + m_bias);

	if (receiver)
		receiver->receiveInput(m_activation, idx);
}

float Neuron::getActivationValue(void) const {
	return m_activation;
}

void Neuron::learn(INeuronDataReceiver *receiver, const float& answer) {
	// TODO: Move m_train_samples in the brain and make it an input value
	float m_train_samples = 1000;

	/* TODO: Move the learn rate to the brain as an input parameter */
	float m_learn_rate = 0.2;

	/*
	 * To gain an understanding of what is being achieved in this function I
	 * recommend reading the following links:
	 *
	 * https://becominghuman.ai/understanding-neural-networks-2-the-math-of-neural-networks-in-3-equations-6085fd3f09df
	 * http://neuralnetworksanddeeplearning.com/chap1.html
	 * https://towardsdatascience.com/machine-learning-fundamentals-via-linear-regression-41a5d11f5220
	 */

	/* Calculate how big error our prediction had */
	float error = m_activation - answer;

	/* We do not take bigger corrections than what we would have taken with
	   1000 train samples. This limits the chance to step over a local
	   minimum. */
	float train_samples = (m_train_samples < 1000) ? 1000 : m_train_samples;

	/* Sum up all the weights */
	float weights_sum = 0.0f;
	for (int i = 0; i < m_weights_len; i++)
		weights_sum += m_weights[i];

	/* Learn and correct the bias and each inputs weight */
	float weights_errors[m_weights_len];
	float cost = 0.0f;
	for (int i = 0; i < m_inputs_len; i++) {

		/* Calculate the amount of error belonging to each weight of
		   this layer */
		weights_errors[i] = (m_weights[i] / weights_sum) * error;

		/* Calculate how far away from a local minimum we are, in order
		   to take a smaller correction step and hopefully not overshoot
		   past the local
			 minimum */
		float gardient = (2 / train_samples) * m_inputs[i] *
				 weights_errors[i];

		/* Calculate the actual error correction */
		float correction = gardient * m_learn_rate;

		cost += pow(answer - (m_inputs[i] * m_weights[i] + m_bias), 2);

		// TODO: what to do with cost function and result
		// TODO: apply weights corrections (and maybe bias?)
	}

	// TODO: Send weights_errors to the receiver layer
	receiver->receiveFeedback(weights_errors);
}

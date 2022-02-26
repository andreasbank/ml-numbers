#ifndef NEURONLAYER_HPP
#define NEURONLAYER_HPP

#include "Neuron.hpp"

class NeuronLayer : public INeuronDataReceiver {
public:
	NeuronLayer();

	void configure(const size_t& inputs_len,
		       const size_t& neurons_len,
		       const bool& first_layer = false);
	void setTrainedData(const float data[], const float biases_data[]);
	virtual void receiveInput(const float& input, const size_t& idx);
	void think(NeuronLayer *next_layer = nullptr);
	std::unique_ptr<float[]> getResults(void) const;
	void getTrainedData(float weights[],
			    size_t& weights_left,
			    float biases[],
			    size_t& biases_left) const;
	void learn(INeuronDataReceiver *receiver = nullptr,
		   const float answers[] = {0});
	void receiveFeedback(const float answers[]);

private:
	static unsigned int c_count;
	unsigned int m_id;
	/** A buffer for the inputs coming from the previous layer. */
	std::shared_ptr<float[]> m_inputs;
	size_t m_inputs_len;
	/** The neurons in this layer. */
	std::unique_ptr<Neuron[]> m_neurons;
	size_t m_neurons_len;
	/** A buffer for the outputs resulting from neuron activity. */
	std::unique_ptr<float[]> m_outputs;
};

#endif /* NEURONLAYER_HPP */

#ifndef NEURON_HPP
#define NEURON_HPP

#include <memory>

/** An interface for objects that should receive neuron outputs. */
class INeuronDataReceiver {
public:
	virtual void receiveInput(const float& input, const size_t& idx) = 0;
	virtual void receiveFeedback(const float answers[]) = 0;
};

class Neuron {
public:
	Neuron();

	static unsigned int getCount(void);
	void configInputs(std::shared_ptr<float[]>& inputs,
			  const size_t& inputs_len,
			  const bool& first_layer = false);
	void setTrainedData(const float data[], const float& bias_data);
	void getTrainedData(float weights[], size_t& weights_len,
			    float &bias) const;
	void think(INeuronDataReceiver *receiver = nullptr,
		   const size_t& idx = 0);
	float getActivationValue(void) const;
	void learn(INeuronDataReceiver *receiver = nullptr,
		   const float& answers = 0);

private:
	static unsigned int c_count;
	unsigned int m_id;
	float m_activation;
	std::shared_ptr<float[]> m_inputs;
	size_t m_inputs_len;
	std::unique_ptr<float[]> m_weights;
	size_t m_weights_len;
	float m_bias;
	float m_last_activation_error;
};

#endif /* NEURON_HPP*/

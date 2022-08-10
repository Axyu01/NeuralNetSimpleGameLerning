#pragma once
template <typename T> class MPList
{//MovingPointerList
	struct ListKey
	{
		ListKey* previousKey;
		ListKey* nextKey;
		T key;
	};
	unsigned int listSize;
	ListKey* firstKey;
	ListKey* lastKey;
	ListKey* pointedKey;
public:
	MPList<T>();
	unsigned int getListSize();
	void addKey(T);
	bool removeKey(unsigned int index);
	bool removeKey(T);
	bool resetPointerKey();
	bool setPtrToLastKey();
	T getPtrKey();
	void setPtrKey(T);
	bool incrementPointer();
	bool decrementPointer();
};
class Neuron
{
public:
	MPList<Neuron*> inputNeuronPointers;
	MPList<Neuron*> outputNeuronPointers;
};
class Function
{
public:
	virtual float function(const float)=0;
	virtual float derivative(const float)=0;
};
class Sigmoid : Function{
	float function(const float);
	float derivative(const float);
};
class LayerNet;
class LayerNeuron :Neuron
{
	friend LayerNet;
	float bias;
	MPList<float> weights;//for input neurons
	Function* function;
public:
	float neuronPartDeriv;
	MPList<Neuron*> inputNeuronPointers;
	MPList<Neuron*> outputNeuronPointers;
	float value;
	LayerNeuron();
	LayerNeuron(Function* function);
	void calculateValue();
	void randomizeParameters();
};
class LayerNet
{

	MPList<LayerNeuron*> inputLayer;
	MPList<LayerNeuron*> outputLayer;
	MPList<MPList<LayerNeuron*>*> hiddenLayersList;
	void connectLayers(MPList<LayerNeuron*>* firstLayer, MPList<LayerNeuron*>* secondLayer);
	void calculateLayer(MPList<LayerNeuron*>* layer);
public:
	float learningRate;
	LayerNet(unsigned int inputSize, unsigned int hiddenLayersNum ,const unsigned int hiddenLayersSize, unsigned int outputSize,float lerningRate);
	void connectNet();
	void setInput(float* inputArray);
	void giveOutput(float* outputArray)
	{
		outputLayer.resetPointerKey();
		for (unsigned int i = 0; i < outputLayer.getListSize(); i++)
		{
			outputArray[i] = outputLayer.getPtrKey()->value;
			outputLayer.incrementPointer();
		}
	}
	void calculateNet();
	float lernByExample(float* desiredOutputArray);
};

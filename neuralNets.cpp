#include "neuralNets.h"
#include "math.h"
#include <iostream>
#include <ctime>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
template<typename T>
MPList<T>::MPList()
{
	listSize = 0;
}
template<typename T>
unsigned int MPList<T>::getListSize()
{
	return listSize;
}
template<typename T>
void MPList<T>::addKey(T key)
{
	ListKey* newKey = new ListKey();
	newKey->key = key;
	if (listSize == 0)
	{
		firstKey = newKey;
		lastKey = newKey;
	}
	else if (listSize > 0)
	{
		newKey->previousKey = lastKey;
		lastKey->nextKey = newKey;
		lastKey = newKey;
	}
	listSize++;
}
template<typename T>
bool MPList<T>::removeKey(T key)
{
	ListKey* keyPtr = firstKey;
	if (listSize > 0)
	{
		if (key == firstKey->key)
		{
			firstKey = firstKey->nextKey;
			delete keyPtr;
			listSize--;
			return true;
		}
	}
	else
	{
		return false;
	}
	while (keyPtr!=lastKey)
	{
		if (key == keyPtr->key)
		{
			keyPtr->previousKey->nextKey = keyPtr->nextKey;
			keyPtr->nextKey->previousKey = keyPtr->previousKey;
			delete keyPtr;
			listSize--;
			return true;
		}
		else
		{
			keyPtr = keyPtr->nextKey;
		}
	}
	if (key == lastKey->key)//keyPtr=lastKey
	{
		lastKey = lastKey->previousKey;
		delete keyPtr;
		listSize--;
		return true;
	}
	return false;
}
template<typename T>
bool MPList<T>::removeKey(unsigned int index)
{
	if (index >=listSize)
	{
		return false;
	}
	ListKey* keyPtr = firstKey;
	if (index==0)
	{
		firstKey = firstKey->nextKey;
		delete keyPtr;
		listSize--;
		return true;
	}
	else if (index==(listSize-1))
	{
		keyPtr = lastKey;
		lastKey = lastKey->previousKey;
		delete keyPtr;
		listSize--;
		return true;
	}
	else
	{
		keyPtr = keyPtr->nextKey;
		for(unsigned int i=1;i==index;i++)
		{
			keyPtr = keyPtr->nextKey;
		}
		keyPtr->previousKey->nextKey = keyPtr->nextKey;
		keyPtr->nextKey->previousKey = keyPtr->previousKey;
		delete keyPtr;
		listSize--;
		return true;
	}
}
template<typename T>
bool MPList<T>::resetPointerKey()
{
	if (listSize > 0)
	{
		pointedKey = firstKey;
		return true;
	}
	return false;
}
template<typename T>
bool MPList<T>::setPtrToLastKey()
{
	if (listSize > 0)
	{
		pointedKey = lastKey;
		return true;
	}
	return false;
}
template<typename T>
T MPList<T>::getPtrKey()
{
	return pointedKey->key;
}
template<typename T>
void MPList<T>::setPtrKey(T key)
{
	 pointedKey->key=key;
}
template<typename T>
bool MPList<T>::incrementPointer()
{
	if (pointedKey != lastKey)
	{
		pointedKey = pointedKey->nextKey;
		return true;
	}
	else
	{
		return false;
	}
}
template<typename T>
bool MPList<T>::decrementPointer()
{
	if (pointedKey != firstKey)
	{
		pointedKey = pointedKey->previousKey;
		return true;
	}
	else
	{
		return false;
	}
}
float Sigmoid::function(const float x)
{
	return 1/(1+exp(-x));
}
float Sigmoid::derivative(const float x)
{
	float y= function(x);
	return y/(1-y);
}
LayerNeuron::LayerNeuron()
{
	bias = 0;
}
LayerNeuron::LayerNeuron(Function* function)
{
	LayerNeuron();
	this->function = function;
}
void LayerNeuron::calculateValue()
{
	value = bias;
	if (inputNeuronPointers.resetPointerKey()&&weights.resetPointerKey())
	{
		for (unsigned int i = 0; i < inputNeuronPointers.getListSize();i++)
		{
			value += ((LayerNeuron*)(inputNeuronPointers.getPtrKey()))->value*weights.getPtrKey();
			inputNeuronPointers.incrementPointer();
			weights.incrementPointer();
		}
		value = function->function(value);
	}
}
void LayerNeuron::randomizeParameters()
{
	//srand(time(NULL));
	bias = rand()%10-5;
	if (weights.resetPointerKey())
	{
		for (unsigned int i = 0; i < weights.getListSize(); i++)
		{
			weights.setPtrKey(rand()%10-5);
			weights.incrementPointer();
		}
	}
}
LayerNet::LayerNet(unsigned int inputSize, unsigned int hiddenLayersNum,unsigned int hiddenLayerSize, unsigned int outputSize,float lerningRate)
{
	this->learningRate = lerningRate;
	while (inputSize > 0)
	{
		inputLayer.addKey(new LayerNeuron((Function*)(new Sigmoid())));
		inputSize--;
	}
	while (outputSize > 0)
	{
		outputLayer.addKey(new LayerNeuron((Function*)(new Sigmoid())));
		outputSize--;
	}
	unsigned int hiddenSize;
	while (hiddenLayersNum > 0)
	{
		MPList<LayerNeuron*>* hiddenLayer = new MPList<LayerNeuron*>();
		hiddenLayersList.addKey(hiddenLayer);
		hiddenSize = hiddenLayerSize;
		while (hiddenSize > 0)
		{
			hiddenLayer->addKey(new LayerNeuron((Function*)(new Sigmoid())));
			hiddenSize--;
		}
		hiddenLayersNum--;
	}
	connectNet();
}
void LayerNet::connectLayers(MPList<LayerNeuron*>* firstLayer, MPList<LayerNeuron*>* secondLayer)
{
	LayerNeuron* temp;
	firstLayer->resetPointerKey();
	for (unsigned int i = 0; i<firstLayer->getListSize(); i++)
	{
		secondLayer->resetPointerKey();
		temp = firstLayer->getPtrKey();
		for (unsigned int j = 0; j<secondLayer->getListSize(); j++)
		{
			secondLayer->getPtrKey()->inputNeuronPointers.addKey((Neuron*)temp);
			secondLayer->getPtrKey()->weights.addKey(1);
			secondLayer->getPtrKey()->randomizeParameters();
			secondLayer->incrementPointer();
		}
		firstLayer->incrementPointer();
	}
}
void LayerNet::connectNet()
{
	MPList<LayerNeuron*>* firstLayer=&inputLayer;
	MPList<LayerNeuron*>* secondLayer = &outputLayer;
	if (hiddenLayersList.getListSize() > 0)
	{
		hiddenLayersList.resetPointerKey();
		bool statement = true;
		while (statement)
		{
			secondLayer = hiddenLayersList.getPtrKey();
			connectLayers(firstLayer, secondLayer);
			firstLayer = secondLayer;
			statement = hiddenLayersList.incrementPointer();
		}
	}
	secondLayer = &outputLayer;
	connectLayers(firstLayer, secondLayer);
}
void LayerNet::setInput(float* inputArray)
{
	inputLayer.resetPointerKey();
	for (unsigned int i = 0; i < inputLayer.getListSize(); i++)
	{
		inputLayer.getPtrKey()->value = inputArray[i];
		inputLayer.incrementPointer();
	}
}
void LayerNet::calculateLayer(MPList<LayerNeuron*>* layer)
{
	layer->resetPointerKey();
	for (unsigned int i = 0; i < layer->getListSize(); i++)
	{
		layer->getPtrKey()->calculateValue();
		layer->incrementPointer();
	}
}
void LayerNet::calculateNet()
{
	hiddenLayersList.resetPointerKey();
	for (unsigned int i = 0; i < hiddenLayersList.getListSize(); i++)
	{
		calculateLayer(hiddenLayersList.getPtrKey());
		hiddenLayersList.incrementPointer();
	}
	calculateLayer(&outputLayer);
}
float LayerNet::lernByExample(float* desiredOutputArray)
{
	float lossFunctionValue = 0;
	outputLayer.resetPointerKey();
	{
		float difference = 0;
		for (unsigned int i = 0; i < outputLayer.getListSize(); i++)
		{
			difference = outputLayer.getPtrKey()->value - desiredOutputArray[i];
			outputLayer.getPtrKey()->neuronPartDeriv = 2 * difference / outputLayer.getListSize();
			lossFunctionValue += difference * difference;
			outputLayer.incrementPointer();
		}
	}
	//By this point all outputPartDeriv are calculated
	lossFunctionValue = lossFunctionValue / outputLayer.getListSize();
	//loss FunctionValue for feedback
	{
		MPList<LayerNeuron*>* hiddenLayer;
		LayerNeuron* neuron;
		hiddenLayersList.resetPointerKey();//prepering neurons for calculations
		for (unsigned int layer = 0; layer < hiddenLayersList.getListSize(); layer++)
		{
			hiddenLayer=hiddenLayersList.getPtrKey();
			hiddenLayer->resetPointerKey();
			for (unsigned int i = 0; i < hiddenLayer->getListSize(); i++)
			{
				hiddenLayer->getPtrKey()->neuronPartDeriv=0;
				hiddenLayer->incrementPointer();
			}
			hiddenLayersList.incrementPointer();
		}
		for (unsigned int i = 0; i < inputLayer.getListSize(); i++)//zbedne
		{
			inputLayer.getPtrKey()->neuronPartDeriv = 0;
			inputLayer.incrementPointer();
		}
		//By this point all partDeriv containers has been properly set to 0
		outputLayer.resetPointerKey();
		for (unsigned int i = 0; i < outputLayer.getListSize(); i++)//zbedne
		{
			LayerNeuron* neuron = outputLayer.getPtrKey();
			float functionValue = neuron->value;
			float neuronDerev = neuron->neuronPartDeriv*neuron->function->derivative(functionValue);
			neuron->bias -= neuronDerev;
			neuron->weights.resetPointerKey();
			neuron->inputNeuronPointers.resetPointerKey();
			for (unsigned int j = 0; j < neuron->weights.getListSize(); j++)
			{
				float weight = neuron->weights.getPtrKey();
				LayerNeuron* downLNeuron = (LayerNeuron*)(neuron->inputNeuronPointers.getPtrKey());
				float value = downLNeuron->value;
				neuron->weights.setPtrKey(weight-neuronDerev*value);
				downLNeuron->neuronPartDeriv += neuronDerev * weight;
				neuron->weights.incrementPointer();
				neuron->inputNeuronPointers.incrementPointer();
			}
			outputLayer.incrementPointer();
		}
		hiddenLayersList.setPtrToLastKey();
		for (unsigned int layer = hiddenLayersList.getListSize()-1; layer==1; layer--)//zbedne
		{
			hiddenLayer = hiddenLayersList.getPtrKey();
			hiddenLayer->resetPointerKey();
			for (unsigned int i = 0; i < hiddenLayer->getListSize(); i++)//zbedne
			{
				LayerNeuron* neuron = hiddenLayer->getPtrKey();
				float functionValue = neuron->value;
				float neuronDerev = neuron->neuronPartDeriv * neuron->function->derivative(functionValue);
				neuron->bias -= neuronDerev;
				neuron->weights.resetPointerKey();
				neuron->inputNeuronPointers.resetPointerKey();
				for (unsigned int j = 0; j < neuron->weights.getListSize(); j++)
				{
					float weight = neuron->weights.getPtrKey();
					LayerNeuron* downLNeuron = (LayerNeuron*)(neuron->inputNeuronPointers.getPtrKey());
					float value = downLNeuron->value;
					neuron->weights.setPtrKey(weight - neuronDerev * value);
					downLNeuron->neuronPartDeriv += neuronDerev * weight;
					neuron->weights.incrementPointer();

					neuron->inputNeuronPointers.incrementPointer();
				}
				hiddenLayer->incrementPointer();
			}
			hiddenLayersList.decrementPointer();
		}
	}
	return lossFunctionValue;
}
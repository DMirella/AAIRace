#include "ai.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "tools.h"

// Neuron
Neuron::Neuron(int count_previous_layer_neurons)
    : count_previous_layer_neurons_(count_previous_layer_neurons) {
  for (int i = 0; i < count_previous_layer_neurons_; i++) {
    weights_.emplace_back(RandomFloat(-0.5, 0.5));
  }
}

double Neuron::GetOuput(const std::vector<double>& inputs) const {
  assert(inputs.size() == weights_.size());
  double result = 0;
  for (int i = 0; i < count_previous_layer_neurons_; i++) {
    result += (weights_[i] * inputs[i]);
  }
  result = ActivationFunction(result);
  return result;
}

double Neuron::ActivationFunction(double value) const {
  return 1.0 / (1.0 + std::exp(-value));
}

// NeuronLayer
NeuronLayer::NeuronLayer(int count_neurons, int count_previous_layer_neurons)
    : count_neurons_(count_neurons) {
  for(int i = 0; i < count_neurons_; i++){
    neurons_.emplace_back(Neuron(count_previous_layer_neurons));
  }
}

std::vector<double> NeuronLayer::GetOutputs(const std::vector<double>& inputs) const {
  std::vector<double> result;
  for (int i = 0; i < count_neurons_; i++) {
    result.emplace_back(neurons_[i].GetOuput(inputs));
  }
  return result;
}

// NeuralNetwork
NeuralNetwork::NeuralNetwork(int count_hidden_layers, int count_hidden_neurons, int count_input_neurons, int count_output_neurons) 
    : count_hidden_layers_(count_hidden_layers)
    , count_hidden_neurons_(count_hidden_neurons)
    , count_input_neurons_(count_input_neurons)
    , count_output_neurons_(count_output_neurons) {
  layers_.emplace_back(NeuronLayer(count_hidden_layers_, count_input_neurons));
  for (int i = 1; i < count_hidden_layers_; i++){
    layers_.emplace_back(NeuronLayer(count_hidden_neurons_, count_hidden_neurons_));
  }
  layers_.emplace_back(NeuronLayer(count_output_neurons_, count_hidden_neurons_));
}

std::vector<double> NeuralNetwork::GetOutputs(const std::vector<double>& inputs) const {
  std::vector<double> current_inputs = inputs, outputs;
  // std::cout << count_hidden_neurons_ << " " << layers_.size() << std::endl;
  for (int i = 0; i < count_hidden_layers_; i++) {
    outputs = layers_[i].GetOutputs(current_inputs);
    current_inputs = std::move(outputs);
  }
  outputs = layers_.back().GetOutputs(current_inputs);
  return outputs;
}
#ifndef AAIRACE_SOURCE_AI_H_
#define AAIRACE_SOURCE_AI_H_

#include <vector>

class Neuron {
 public:
  Neuron(int count_previous_layer_neurons);

  double GetOuput(const std::vector<double>& inputs) const;
 private:
  double ActivationFunction(double value) const;

  std::vector<double> weights_;
  int count_previous_layer_neurons_;
};

class NeuronLayer {
 public:
  NeuronLayer(int count_neurons, int count_previous_layer_neurons);
  std::vector<double> GetOutputs(const std::vector<double>& inputs) const;
 private:
  int count_neurons_;
  std::vector<Neuron> neurons_;
};

class NeuralNetwork {
 public:
  NeuralNetwork(int count_hidden_layers, int count_hidden_neurons, int count_input_neurons, int count_output_neurons);
  std::vector<double> GetOutputs(const std::vector<double>& inputs) const;
 private:
  int count_hidden_layers_;
  int count_hidden_neurons_;
  int count_input_neurons_;
  int count_output_neurons_;
  std::vector<NeuronLayer> layers_;
};

#endif  // AAIRACE_SOURCE_AI_H_
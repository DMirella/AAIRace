#ifndef AAIRACE_SOURCE_AI_H_
#define AAIRACE_SOURCE_AI_H_

#include <memory>
#include <vector>

namespace MiniDNN {
class Layer;
class Network;
}  // namespace MiniDNN

class NeuralNetwork {
 public:
  NeuralNetwork(int count_hidden_layers, int count_hidden_neurons, int count_input_neurons, int count_output_neurons);
  std::vector<double> GetOutputs(const std::vector<double>& inputs) const;
  void Train(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& train_matrix);
 private:
  std::vector<MiniDNN::Layer*> layers_;
  MiniDNN::Layer* output_layer_;
  std::shared_ptr<MiniDNN::Network> network_;
  int count_output_neurons_;
};

#endif  // AAIRACE_SOURCE_AI_H_
#ifndef AAIRACE_SOURCE_AI_H_
#define AAIRACE_SOURCE_AI_H_

#include <memory>
#include <vector>

#include <MiniDNN.h>

namespace ai {
class NeuralNetwork {
 public:
  NeuralNetwork(const NeuralNetwork& neural_network) = delete;
  NeuralNetwork(NeuralNetwork&& neural_network) = delete;
  NeuralNetwork& operator=(const NeuralNetwork& neural_network) = delete;
  NeuralNetwork& operator=(NeuralNetwork&& neural_network) = delete;

  NeuralNetwork() = default;
  NeuralNetwork(int count_hidden_layers, int count_hidden_neurons,
                int count_input_neurons, int count_output_neurons);
  
  std::vector<double> GetOutputs(const std::vector<double>& inputs);
  void Train(const std::vector<std::pair<std::vector<double>,
             std::vector<double>>>& train_matrix);
  void SaveToFile(const std::string& path, const std::string& file_name);
  void LoadFromFile(const std::string& path, const std::string& file_name);
 private:
  MiniDNN::Network network_;
};
}  // namespace ai

#endif  // AAIRACE_SOURCE_AI_H_
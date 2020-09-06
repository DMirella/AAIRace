#include "ai.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <common/tools.h>

namespace ai {
NeuralNetwork::NeuralNetwork(int count_hidden_layers, int count_hidden_neurons,
                             int count_input_neurons, int count_output_neurons) {
  network_.add_layer(
      new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_input_neurons, count_hidden_neurons));
  for (int i = 0; i < count_hidden_layers - 1; i++) {
    network_.add_layer(
        new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_hidden_neurons, count_hidden_neurons));
  }
  network_.add_layer(
      new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_hidden_neurons, count_output_neurons));
  network_.set_output(new MiniDNN::RegressionMSE());
  network_.set_default_callback();
  network_.init(0, 1, std::rand());
}

std::vector<double> NeuralNetwork::GetOutputs(const std::vector<double>& inputs) {
  Eigen::VectorXd input_vector(inputs.size());
  for (int i = 0; i < inputs.size(); i++) {
    input_vector(i) = inputs[i];
  }
  auto matrix_result = network_.predict(input_vector);
  std::vector<double> result;
  for (int i = 0; i < matrix_result.rows(); i++) {
    result.push_back(matrix_result(i, 0));
  }
  return result;
}

void NeuralNetwork::Train(
    const std::vector<std::pair<std::vector<double>, std::vector<double>>>& train_matrix) {
  assert(train_matrix.size() > 0);

  const int kMaxLearnIterations = 100000;
  const double kLearningRate = 0.01;
  const int kBatchSize = std::max(static_cast<int>(train_matrix.size()) / 10, 1);
  const int kEpochCount = std::ceil(static_cast<float>(kMaxLearnIterations) / train_matrix.size());

  Eigen::MatrixXd inputs_train_matrix(train_matrix[0].first.size(), train_matrix.size());
  Eigen::MatrixXd outputs_train_matrix(train_matrix[0].second.size(), train_matrix.size());
  for (int i = 0; i < train_matrix.size(); i++) {
    for (int j = 0; j < train_matrix[i].first.size(); j++) {
      inputs_train_matrix(j, i) = train_matrix[i].first[j];
    }
  }
  for (int i = 0; i < train_matrix.size(); i++) {
    for (int j = 0; j < train_matrix[i].second.size(); j++) {
      outputs_train_matrix(j, i) = train_matrix[i].second[j];
    }
  }
  
  MiniDNN::RMSProp rms_prop;
  rms_prop.m_lrate = kLearningRate;
  network_.fit(rms_prop, inputs_train_matrix, outputs_train_matrix, kBatchSize, kEpochCount, std::rand());
  auto outs = network_.predict(inputs_train_matrix);
  float cnt = 0;
  for (int i = 0; i < train_matrix.size(); i++) {
    for (int j = 0; j < 4; j++) {
      if (outputs_train_matrix(j, i) == 1 && outs(j, i) > 0.5) {
        cnt++;
      }
      if (outputs_train_matrix(j, i) == 0 && outs(j, i) < 0.5) {
        cnt++;
      }
    }
  }
  std::cout << "Quality: " << cnt / train_matrix.size() << std::endl;
}

void NeuralNetwork::SaveToFile(const std::string& path, const std::string& file_name) {
  network_.export_net(path, file_name);
}

void NeuralNetwork::LoadFromFile(const std::string& path, const std::string& file_name) {
  network_.read_net(path, file_name);
}
}  // namespace ai
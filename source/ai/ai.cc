#include "ai.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <MiniDNN.h>

#include <common/tools.h>
/*
namespace {
const float gMutationChance = 0.03f;
}  // namespace

// Neuron
Neuron::Neuron(int count_previous_layer_neurons)
    : count_previous_layer_neurons_(count_previous_layer_neurons) {
  for (int i = 0; i < count_previous_layer_neurons_; i++) {
    weights_.emplace_back(GetRandomNormilizedFloat());
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

Neuron Neuron::MergeNeurons(const Neuron& a, const Neuron& b) {
  assert(a.weights_.size() == b.weights_.size());
  Neuron result = a;
  for (int i = 0; i < a.weights_.size(); i++) {
    if (RandomFloat(0.0f, 1.0f) > 0.5f) {
      result.weights_[i] = b.weights_[i];
    }
    if (RandomFloat(0.0f, 1.0f) <= gMutationChance) {
      result.weights_[i] = GetRandomNormilizedFloat();
    }
  }
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

NeuronLayer NeuronLayer::MergeNeuronLayers(const NeuronLayer& a, const NeuronLayer& b) {
  assert(a.neurons_.size() == b.neurons_.size());
  NeuronLayer result = a;
  for (int i = 0; i < result.neurons_.size(); i++) {
    result.neurons_[i] = Neuron::MergeNeurons(result.neurons_[i], b.neurons_[i]);
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

NeuralNetwork NeuralNetwork::MergeNeuralNetworks(const NeuralNetwork& a, const NeuralNetwork& b) {
  assert(a.layers_.size() == b.layers_.size());
  NeuralNetwork result = a;
  for (int i = 0; i < result.layers_.size(); i++) {
    result.layers_[i] = NeuronLayer::MergeNeuronLayers(result.layers_[i], b.layers_[i]);
  }
  return result;
}*/

NeuralNetwork::NeuralNetwork(int count_hidden_layers, int count_hidden_neurons, int count_input_neurons, int count_output_neurons)
    : count_output_neurons_(count_output_neurons) {
  network_ = std::make_shared<MiniDNN::Network>();
  layers_.emplace_back(new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_input_neurons, count_hidden_neurons));
  for (int i = 0; i < count_hidden_layers - 1; i++) {
    layers_.emplace_back(new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_hidden_neurons, count_hidden_neurons));
  }
  layers_.emplace_back(new MiniDNN::FullyConnected<MiniDNN::Sigmoid>(count_hidden_neurons, count_output_neurons));

  for (const auto& current_layer : layers_) {
    network_->add_layer(current_layer);
  }
  network_->set_output(new MiniDNN::RegressionMSE());
  network_->set_default_callback();
  network_->init(0, 1, std::rand());
}

std::vector<double> NeuralNetwork::GetOutputs(const std::vector<double>& inputs) const {
  Eigen::VectorXd input_vector(inputs.size());
  for (int i = 0; i < inputs.size(); i++) {
    input_vector(i) = inputs[i];
  }
  auto matrix_result = network_->predict(input_vector);
  std::vector<double> result;
  for (int i = 0; i < count_output_neurons_; i++) {
    result.push_back(matrix_result(i, 0));
    // std::cout << result.back() << " ";
  }
  // std::cout << "\n";
  return result;
}

void NeuralNetwork::Train(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& train_matrix) {
  assert(train_matrix.size() > 0);

  const double kLearningRate = 0.001;
  const int kBatchSize = std::max(static_cast<int>(train_matrix.size()) / 10, 1);
  const int kEpochCount = 10;

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
  std::cout << train_matrix.size() << std::endl;
  std::cout << "Inputs: " << inputs_train_matrix.rows() << " " << inputs_train_matrix.cols() << std::endl;
  std::cout << "Outputs: " << outputs_train_matrix.rows() << " " << outputs_train_matrix.cols() << std::endl;
  std::cout << "--------------------------\n";
  MiniDNN::RMSProp rms_prop;
  rms_prop.m_lrate = kLearningRate;
  network_->fit(rms_prop, inputs_train_matrix, outputs_train_matrix, kBatchSize, kEpochCount, std::rand());
  auto outs = network_->predict(inputs_train_matrix);
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
#include "enemy_ai.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <list>
#include <iostream>

#include <common/tools.h>

namespace {
const int gCountHiddenLayers = 1;
const int gCountHiddenNeurons = 120;
const int gCountInputNeurons = ai::AIInputData::kInputCount;
const int gCountOutputNeurons = ai::AIOutputData::kOutputCount;

double sqr(double a) {
  return a * a;
}
}

namespace ai {
void EnemyAI::Init() {
  neural_network_ = std::make_unique<NeuralNetwork>(
                        gCountHiddenLayers, gCountHiddenNeurons,
                        gCountInputNeurons, gCountOutputNeurons);
}

void EnemyAI::TrainWithData(const std::vector<AIIOData>& aiio_data) {
  std::vector<std::pair<std::vector<double>, std::vector<double>>> train_matrix;
  const int kMaxTrainMatrixSize = 12000;
  for (const auto& it : aiio_data) {
    train_matrix.push_back(std::make_pair(TransformAIInputDataToVector(it.input),
                                          TransformAIOutputDataToVector(it.output)));
    if (train_matrix.size() > kMaxTrainMatrixSize) {
      break;
    }
  }
  
  // std::cout << train_matrix.size() << std::endl;
  if (train_matrix.size() > 0) {
    neural_network_->Train(train_matrix);
  }
}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) {
  auto vector_outputs = neural_network_->GetOutputs(TransformAIInputDataToVector(input_data));
  return TransformVectorToAIOutputData(vector_outputs);
}

void EnemyAI::SaveToFile(const std::string& path, const std::string& file_name) {
  neural_network_->SaveToFile(path, file_name);
}

void EnemyAI::InitFromFile(const std::string& path, const std::string& file_name) {
  neural_network_ = std::make_unique<NeuralNetwork>();
  neural_network_->LoadFromFile(path, file_name);
}
}  // namespace ai
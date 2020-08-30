#include "enemy_ai.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <list>
#include <iostream>

#include <common/tools.h>

namespace {
const int gCountHiddenLayers = 1;
const int gCountHiddenNeurons = 30;
const int gCountInputNeurons = AIInputData::kInputCount;
const int gCountOutputNeurons = AIOutputData::kOutputCount;

double sqr(double a) {
  return a * a;
}
}

// EnemyAI
EnemyAI::EnemyAI(const std::shared_ptr<std::vector<AIIOData>>& collected_aiio_data)
    : neural_network_(gCountHiddenLayers, gCountHiddenNeurons, gCountInputNeurons, gCountOutputNeurons)
    , last_applied_action_index_(-1)
    , collected_aiio_data_(collected_aiio_data) {
}

void EnemyAI::TrainWithData(const std::vector<AIIOData>& aiio_data) {
  std::vector<AIIOData> train_aiio_data = FilterAIIOData(aiio_data);
  std::vector<std::pair<std::vector<double>, std::vector<double>>> train_matrix;
  for (const auto& it : train_aiio_data) {
    train_matrix.push_back(std::make_pair(TransformAIInputDataToVector(it.input), TransformAIOutputDataToVector(it.output)));
  }
  neural_network_.Train(train_matrix);
}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) {
  const int kSearchFromLastActionRange = 2000;
  if (nullptr != collected_aiio_data_ && collected_aiio_data_->size() > 0) {
    std::pair<double, int> minimum_distance = {tools::gInfinity, 0};
    int left_search_range = 0, right_search_range = collected_aiio_data_->size() - 1;
    if (last_applied_action_index_ != -1) {
      left_search_range = std::max(last_applied_action_index_ - kSearchFromLastActionRange, 0);
      right_search_range = std::min(last_applied_action_index_ + kSearchFromLastActionRange, 
                                    static_cast<int>(collected_aiio_data_->size()) - 1);
    }
    for (int i = left_search_range; i <= right_search_range; i++) {
      double current_distance = 0.0;
      for (int j = 0; j < AIInputData::kCountDistanceScanRays; j++) {
        current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_city_cars[j].distance - input_data.distance_to_city_cars[j].distance);
        current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_city_cars[j].speed - input_data.distance_to_city_cars[j].speed);
      }
      for (int j = 0; j < AIInputData::kCountDistanceScanRays; j++) {
        current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_racing_cars[j].distance - input_data.distance_to_racing_cars[j].distance);
        current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_racing_cars[j].speed - input_data.distance_to_racing_cars[j].speed);
      }
      current_distance += sqr((*collected_aiio_data_)[i].input.current_speed - input_data.current_speed);
      current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_left_border - input_data.distance_to_left_border);
      current_distance += sqr((*collected_aiio_data_)[i].input.distance_to_right_border - input_data.distance_to_right_border);

      if (current_distance < minimum_distance.first) {
        minimum_distance = {current_distance, i};
      }
    }
    last_applied_action_index_ = minimum_distance.second;
    return (*collected_aiio_data_)[minimum_distance.second].output;
  }
  return {0, 0, 0, 0};
  /*
  auto vector_outputs = neural_network_.GetOutputs(TransformAIInputDataToVector(input_data));
  return TransformVectorToAIOutputData(vector_outputs);
  */
}
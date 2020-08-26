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
const int gCountInputNeurons = AIInputData::kCountDistanceScanRays * 2 * 2 + 3;
const int gCountOutputNeurons = 4;

double sqr(double a) {
  return a * a;
}
}

EnemyAI::EnemyAI() 
    : neural_network_(NeuralNetwork(gCountHiddenLayers, gCountHiddenNeurons, gCountInputNeurons, gCountOutputNeurons)) {

}

void EnemyAI::TrainWithData(const std::vector<AIIOData>& aiio_data) {
  std::vector<AIIOData> train_aiio_data = GetTrainData(aiio_data);
  std::vector<std::pair<std::vector<double>, std::vector<double>>> train_matrix;
  for (const auto& it : train_aiio_data) {
    train_matrix.push_back(std::make_pair(TransformAIInputDataToVector(it.input), TransformAIOutputDataToVector(it.output)));
  }
 // neural_network_.Train(train_matrix);
  last_aiio_data_ = aiio_data;
}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) const {
  if (last_aiio_data_.size() > 0) {
    std::pair<double, int> minimum_distance = {101010101, 0};
    for (int i = 0; i < last_aiio_data_.size(); i++) {
      double current_distance = 0.0;
      for (int j = 0; j < AIInputData::kCountDistanceScanRays; j++) {
        current_distance += sqr(last_aiio_data_[i].input.distance_to_city_cars[j].distance - input_data.distance_to_city_cars[j].distance);
        current_distance += sqr(last_aiio_data_[i].input.distance_to_city_cars[j].speed - input_data.distance_to_city_cars[j].speed);
      }
      for (int j = 0; j < AIInputData::kCountDistanceScanRays; j++) {
        current_distance += sqr(last_aiio_data_[i].input.distance_to_racing_cars[j].distance - input_data.distance_to_racing_cars[j].distance);
        current_distance += sqr(last_aiio_data_[i].input.distance_to_racing_cars[j].speed - input_data.distance_to_racing_cars[j].speed);
      }
      current_distance += sqr(last_aiio_data_[i].input.current_speed - input_data.current_speed);
      current_distance += sqr(last_aiio_data_[i].input.distance_to_left_border - input_data.distance_to_left_border);
      current_distance += sqr(last_aiio_data_[i].input.distance_to_right_border - input_data.distance_to_right_border);

      if (current_distance < minimum_distance.first) {
        minimum_distance = {current_distance, i};
      }
    }
    return last_aiio_data_[minimum_distance.second].output;
  } else {
    return {0, 0, 0, 0};
  }

  // TransformAIInputDataToVector(input_data);
  auto vector_outputs = neural_network_.GetOutputs(TransformAIInputDataToVector(input_data));
  return TransformVectorToAIOutputData(vector_outputs);
  // return {0, 0, 0, 0};
}

std::vector<double> EnemyAI::TransformAIInputDataToVector(const AIInputData& ai_input_data) const {
  std::vector<double> result;
  result.push_back(ai_input_data.current_speed);
  result.push_back(ai_input_data.distance_to_left_border);
  result.push_back(ai_input_data.distance_to_right_border);
  for (int i = 0; i < AIInputData::kCountDistanceScanRays; i++) {
    result.push_back(ai_input_data.distance_to_racing_cars[i].distance);
    result.push_back(ai_input_data.distance_to_racing_cars[i].speed);
  }
  for (int i = 0; i < AIInputData::kCountDistanceScanRays; i++) {
    result.push_back(ai_input_data.distance_to_city_cars[i].distance);
    result.push_back(ai_input_data.distance_to_city_cars[i].speed);
  }
  return result;
}

std::vector<double> EnemyAI::TransformAIOutputDataToVector(const AIOutputData& ai_output_data) const {
  std::vector<double> result;
  result.push_back(ai_output_data.is_accelerate_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_slow_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_turn_left_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_turn_right_action_active? 1 : 0);
  return result;
}

AIOutputData EnemyAI::TransformVectorToAIOutputData(const std::vector<double>& vector) const {
  assert(vector.size() == gCountOutputNeurons);

  const double kTrueThreshold = 0.5;

  AIOutputData result;
  result.is_accelerate_action_active = (vector[0] > kTrueThreshold);
  result.is_car_slow_action_active = (vector[1] > kTrueThreshold);
  result.is_car_turn_left_action_active = (vector[2] > kTrueThreshold);
  result.is_car_turn_right_action_active = (vector[3] > kTrueThreshold);
  return result;
}

std::vector<AIIOData> EnemyAI::GetTrainData(const std::vector<AIIOData>& aiio_data) const {
  const int kMinimumActionDatasetSize = 50;

  std::pair<int, int> output_action_count[gCountOutputNeurons];
  for (int i = 0; i < gCountOutputNeurons; i++) {
    output_action_count[i] = std::make_pair(0, i);
  }

  for (const auto& it : aiio_data) {
    for (int i = 0; i < gCountOutputNeurons; i++) {
      if (GetValueFromActionNumber(it.output, i)) {
        output_action_count[i].first++;
      }
    }
  }

  int minimum_action_count = tools::gInfinity;
  std::sort(output_action_count, output_action_count + gCountOutputNeurons);
  for (int i = 0; i < gCountOutputNeurons; i++) {
    if (output_action_count[i].first >= kMinimumActionDatasetSize) {
      minimum_action_count = std::min(minimum_action_count, output_action_count[i].first);
    }
  }

  std::list<AIIOData> aiio_data_list(aiio_data.begin(), aiio_data.end());

  std::vector<AIIOData> result_for_each_action[gCountOutputNeurons];
  int taken_output_action_count[gCountOutputNeurons];
  std::fill(taken_output_action_count, taken_output_action_count + gCountOutputNeurons, 0);
  for (int i = 0; i < gCountOutputNeurons; i++) {
    std::list<AIIOData>::iterator it = aiio_data_list.begin();
    const auto current_collecting_action = output_action_count[i].second;
    while (it != aiio_data_list.end()) {
      if (taken_output_action_count[current_collecting_action] >= minimum_action_count) {
        break;
      }
      if (GetValueFromActionNumber(it->output, current_collecting_action)) {
        result_for_each_action[i].push_back(*it);
        for (int j = 0; j < gCountOutputNeurons; j++) {
          if (GetValueFromActionNumber(it->output, j)) {
            taken_output_action_count[j]++;
          }
        }
        it = aiio_data_list.erase(it);
      } else {
        ++it;
      }
    }
  }

  std::vector<AIIOData> shuffled_result;
  std::vector<int> available_indexes;
  for (int i = 0; i < gCountOutputNeurons; i++) {
    // std::cout << "H: "<< output_action_count[i].second << " " << result_for_each_action[i].size() << std:: endl;;
    if (result_for_each_action[i].size() > 0) {
      available_indexes.push_back(i);
    }
  }
  
  while (!available_indexes.empty()) {
    int random_index = std::rand() % available_indexes.size();
    int current_index = available_indexes[random_index];
    // std::cout << current_index << " " << result_for_each_action[current_index].size();
    shuffled_result.emplace_back(result_for_each_action[current_index].back());
    result_for_each_action[current_index].pop_back();
    if (result_for_each_action[current_index].size() == 0) {
      auto it = std::find(available_indexes.begin(), available_indexes.end(), current_index);
      available_indexes.erase(it);
    }
  }

  return shuffled_result;
}

bool EnemyAI::GetValueFromActionNumber(const AIOutputData& data, int number) const {
  switch (number) {
    case 0:
      return data.is_accelerate_action_active;
    case 1:
      return data.is_car_slow_action_active;
    case 2:
      return data.is_car_turn_left_action_active;
    case 3:
      return data.is_car_turn_right_action_active;
  }
  return false;
}
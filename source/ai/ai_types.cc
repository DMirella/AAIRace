#include "ai_types.h"

#include <algorithm>
#include <list>

#include <common/tools.h>

std::vector<double> TransformAIInputDataToVector(const AIInputData& ai_input_data) {
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

std::vector<double> TransformAIOutputDataToVector(const AIOutputData& ai_output_data) {
  std::vector<double> result;
  result.push_back(ai_output_data.is_accelerate_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_slow_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_turn_left_action_active? 1 : 0);
  result.push_back(ai_output_data.is_car_turn_right_action_active? 1 : 0);
  return result;
}

AIInputData TransformVectorToAIInputData(const std::vector<double>& vector) {
  AIInputData result;
  std::vector<double>::const_iterator it = vector.begin();
  result.current_speed = *it++;
  result.distance_to_left_border = *it++;
  result.distance_to_right_border = *it++;
  for (int i = 0; i < AIInputData::kCountDistanceScanRays; i++) {
    result.distance_to_racing_cars[i].distance = *it++;
    result.distance_to_racing_cars[i].speed = *it++;
  }
  for (int i = 0; i < AIInputData::kCountDistanceScanRays; i++) {
    result.distance_to_city_cars[i].distance = *it++;
    result.distance_to_city_cars[i].speed = *it++;
  }
  return result;
}

AIOutputData TransformVectorToAIOutputData(const std::vector<double>& vector) {
  const double kTrueThreshold = 0.5;
  AIOutputData result;
  result.is_accelerate_action_active = (vector[0] > kTrueThreshold);
  result.is_car_slow_action_active = (vector[1] > kTrueThreshold);
  result.is_car_turn_left_action_active = (vector[2] > kTrueThreshold);
  result.is_car_turn_right_action_active = (vector[3] > kTrueThreshold);
  return result;
}

bool GetValueFromActionNumber(const AIOutputData& data, int number) {
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

std::vector<AIIOData> FilterAIIOData(const std::vector<AIIOData>& aiio_data) {
  const int kMinimumActionDatasetSize = 10;

  std::pair<int, int> output_action_count[AIOutputData::kOutputCount];
  for (int i = 0; i < AIOutputData::kOutputCount; i++) {
    output_action_count[i] = std::make_pair(0, i);
  }

  for (const auto& it : aiio_data) {
    for (int i = 0; i < AIOutputData::kOutputCount; i++) {
      if (GetValueFromActionNumber(it.output, i)) {
        output_action_count[i].first++;
      }
    }
  }

  int minimum_action_count = tools::gInfinity;
  std::sort(output_action_count, output_action_count + AIOutputData::kOutputCount);
  for (int i = 0; i < AIOutputData::kOutputCount; i++) {
    if (output_action_count[i].first >= kMinimumActionDatasetSize) {
      minimum_action_count = std::min(minimum_action_count, output_action_count[i].first);
    }
  }

  std::list<AIIOData> aiio_data_list(aiio_data.begin(), aiio_data.end());

  std::vector<AIIOData> result_for_each_action[AIOutputData::kOutputCount];
  int taken_output_action_count[AIOutputData::kOutputCount];
  std::fill(taken_output_action_count, taken_output_action_count + AIOutputData::kOutputCount, 0);
  for (int i = 0; i < AIOutputData::kOutputCount; i++) {
    std::list<AIIOData>::iterator it = aiio_data_list.begin();
    const auto current_collecting_action = output_action_count[i].second;
    while (it != aiio_data_list.end()) {
      if (taken_output_action_count[current_collecting_action] >= minimum_action_count) {
        break;
      }
      if (GetValueFromActionNumber(it->output, current_collecting_action)) {
        result_for_each_action[i].push_back(*it);
        for (int j = 0; j < AIOutputData::kOutputCount; j++) {
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
  for (int i = 0; i < AIOutputData::kOutputCount; i++) {
    if (result_for_each_action[i].size() > 0) {
      available_indexes.push_back(i);
    }
  }
  
  while (!available_indexes.empty()) {
    int random_index = std::rand() % available_indexes.size();
    int current_index = available_indexes[random_index];
    shuffled_result.emplace_back(result_for_each_action[current_index].back());
    result_for_each_action[current_index].pop_back();
    if (result_for_each_action[current_index].size() == 0) {
      auto it = std::find(available_indexes.begin(), available_indexes.end(), current_index);
      available_indexes.erase(it);
    }
  }

  return shuffled_result;
}
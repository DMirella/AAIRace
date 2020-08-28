#include "ai_types.h"

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
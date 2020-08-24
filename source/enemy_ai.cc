#include "enemy_ai.h"

#include <cassert>

namespace {
const int gCountHiddenLayers = 2;
const int gCountHiddenNeurons = 6;
const int gCountInputNeurons = AIInputData::kCountClosestMemorizedCityCars * 3 + AIInputData::kCountClosestMemorizedRacingCars * 3 + 3;
const int gCountOutputNeurons = 4;
}

EnemyAI::EnemyAI() 
    : neural_network_(NeuralNetwork(gCountHiddenLayers, gCountHiddenNeurons, gCountInputNeurons, gCountOutputNeurons)) {

}

void EnemyAI::TrainWithData(const std::vector<AIIOData>& aiio_data) {

}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) const {
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
  for (int i = 0; i < AIInputData::kCountClosestMemorizedCityCars; i++) {
    result.push_back(ai_input_data.closest_city_cars[i].angle);
    result.push_back(ai_input_data.closest_city_cars[i].distance);
    result.push_back(ai_input_data.closest_city_cars[i].speed);
  }
  for (int i = 0; i < AIInputData::kCountClosestMemorizedRacingCars; i++) {
    result.push_back(ai_input_data.closest_racing_cars[i].angle);
    result.push_back(ai_input_data.closest_racing_cars[i].distance);
    result.push_back(ai_input_data.closest_racing_cars[i].speed);
  }
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
#include "enemy_ai.h"

#include <cassert>
#include <iostream>

namespace {
const int gCountHiddenLayers = 1;
const int gCountHiddenNeurons = 15;
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
  std::vector<std::pair<std::vector<double>, std::vector<double>>> train_matrix;
  for (const auto& it : aiio_data) {
    train_matrix.push_back(std::make_pair(TransformAIInputDataToVector(it.input), TransformAIOutputDataToVector(it.output)));
  }
  neural_network_.Train(train_matrix);
  // last_aiio_data_ = aiio_data;
}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) const {
  /*if (last_aiio_data_.size() > 0) {
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
  }*/

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
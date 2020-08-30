#ifndef AAIRACE_SOURCE_AI_TYPES_H_
#define AAIRACE_SOURCE_AI_TYPES_H_

#include <vector>

namespace ai {
struct AIInputData {
  static const int kCountDistanceScanRays = 20;
  static const int kInputCount = kCountDistanceScanRays * 2 * 2 + 3;
  struct OtherCarRegardingCurrentCarInfo {
    float speed;
    float distance;
  };

  OtherCarRegardingCurrentCarInfo distance_to_city_cars[kCountDistanceScanRays];
  OtherCarRegardingCurrentCarInfo distance_to_racing_cars[kCountDistanceScanRays];

  float distance_to_left_border;
  float distance_to_right_border;
  float current_speed;
};

struct AIOutputData {
  static const int kOutputCount = 4;
  bool is_accelerate_action_active;
  bool is_car_slow_action_active;
  bool is_car_turn_left_action_active;
  bool is_car_turn_right_action_active;
};

struct AIIOData {
  AIInputData input;
  AIOutputData output;
};

std::vector<double> TransformAIInputDataToVector(const AIInputData& ai_input_data);
std::vector<double> TransformAIOutputDataToVector(const AIOutputData& ai_output_data);
AIInputData TransformVectorToAIInputData(const std::vector<double>& vector);
AIOutputData TransformVectorToAIOutputData(const std::vector<double>& vector);
bool GetValueFromActionNumber(const AIOutputData& data, int number);
std::vector<AIIOData> FilterAIIOData(const std::vector<AIIOData>& aiio_data);
}  // namespace ai

#endif  // AAIRACE_SOURCE_AI_TYPES_H_
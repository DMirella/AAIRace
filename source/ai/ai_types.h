#ifndef AAIRACE_SOURCE_AI_TYPES_H_
#define AAIRACE_SOURCE_AI_TYPES_H_

struct AIInputData {
  struct OtherCarRegardingCurrentCarInfo {
    float speed;
    float distance;
  };

  static const int kCountDistanceScanRays = 20;
  OtherCarRegardingCurrentCarInfo distance_to_city_cars[kCountDistanceScanRays];
  OtherCarRegardingCurrentCarInfo distance_to_racing_cars[kCountDistanceScanRays];

  float distance_to_left_border;
  float distance_to_right_border;
  float current_speed;
};

struct AIOutputData {
  bool is_accelerate_action_active;
  bool is_car_slow_action_active;
  bool is_car_turn_left_action_active;
  bool is_car_turn_right_action_active;
};

struct AIIOData {
  AIInputData input;
  AIOutputData output;
};

#endif  // AAIRACE_SOURCE_AI_TYPES_H_
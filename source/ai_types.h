#ifndef AAIRACE_SOURCE_AI_TYPES_H_
#define AAIRACE_SOURCE_AI_TYPES_H_

struct AIInputData {
  struct OtherCarRegardingCurrentCarInfo {
    float speed;
    float distance;
    float angle;
  };

  static const int kCountClosestMemorizedCityCars = 10;
  static const int kCountClosestMemorizedRacingCars = 2;
  OtherCarRegardingCurrentCarInfo closest_city_cars[kCountClosestMemorizedCityCars];
  OtherCarRegardingCurrentCarInfo closest_racing_cars[kCountClosestMemorizedRacingCars];

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
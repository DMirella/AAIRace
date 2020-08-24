#ifndef AAIRACE_SOURCE_RACING_CAR_H_
#define AAIRACE_SOURCE_RACING_CAR_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include "car.h"

class RacingCar : public Car {
 public:
  RacingCar() = delete;
  RacingCar(const RacingCar& racing_car) = delete;
  RacingCar(RacingCar&& racing_car) = delete;
  RacingCar&& operator=(const RacingCar& racing_car) = delete;
  RacingCar&& operator=(RacingCar&& racing_car) = delete;

  RacingCar(int x, int y, const DrawFunction& draw_function);

  static void Init();

  // Controls
  void Accelerate();
  void SlowDown();
  void TurnLeft();
  void TurnRight();

  void SetBlockMove(bool is_blocked);

  // DrawableUnit
  virtual void Update(float elapsed_time) override;
  virtual void Draw() override;

  // Car
  virtual void SetHeroCar(const std::shared_ptr<Car>& hero_car) override;
  virtual Rectangle GetIntersectRectangle() const override;

  void GetControllersState(bool* const is_car_accelerates,
                           bool* const is_car_slows_down,
                           bool* const is_car_turns_left,
                           bool* const is_car_turns_right);

 private:
  void ResetControls();

  static sf::Texture texture_;

  const float kDefaultSpeed = 0.3f;
  const float kAccelerateValue = 0.00001f;
  const float kSlowDownValue = 0.0001f;
  const float kDefaultSlowDown = 0.00006f;
  const float kMaxCarSpeed = kDefaultSpeed + 0.2f;
  const float kMinCarSpeed = kDefaultSpeed - 0.15f;
  const float kTurnSpeed = 0.1f;

  bool is_car_accelerate_now_;
  bool is_car_slow_down_now_;
  bool is_car_turn_left_now_;
  bool is_car_turn_right_now_;

  bool is_move_blocked_;
};

#endif  // AAIRACE_SOURCE_RACING_CAR_H_
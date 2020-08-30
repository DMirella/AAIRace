#ifndef AAIRACE_SOURCE_GAME_UNITS_RACING_CAR_H_
#define AAIRACE_SOURCE_GAME_UNITS_RACING_CAR_H_

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
  static float height();
  static float max_speed();

  // Controls
  void Accelerate();
  void SlowDown();
  void TurnLeft();
  void TurnRight();

  // DrawableUnit
  virtual void Update(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  // Car
  virtual void SetHeroCar(const std::shared_ptr<Car>& hero_car) override;
  virtual tools::Rectangle GetIntersectRectangle() const override;

  void GetControllersState(bool* const is_car_accelerates,
                           bool* const is_car_slows_down,
                           bool* const is_car_turns_left,
                           bool* const is_car_turns_right);
 private:
  void ResetControls();

  static sf::Texture texture_;

  bool is_car_accelerate_now_;
  bool is_car_slow_down_now_;
  bool is_car_turn_left_now_;
  bool is_car_turn_right_now_;
};

#endif  // AAIRACE_SOURCE_GAME_UNITS_RACING_CAR_H_
#ifndef AAIRACE_SOURCE_GAME_UNITS_CITY_CAR_H_
#define AAIRACE_SOURCE_GAME_UNITS_CITY_CAR_H_

#include <SFML/Graphics.hpp>

#include "car.h"

namespace game {
namespace units {
class RacingCar;
class CityCar : public Car {
 public:
  CityCar() = delete;
  CityCar(const CityCar& city_car) = delete;
  CityCar(CityCar&& city_car) = delete;
  CityCar&& operator=(const CityCar& city_car) = delete;
  CityCar&& operator=(CityCar&& city_car) = delete;

  CityCar(int x, int y, const DrawFunction& draw_function);

  static void Init();
  static float height();

  // DrawableUnit
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;
  // Car
  virtual common::Rectangle GetIntersectRectangle() const override;
 private:
  static sf::Texture texture_;
};
}  // namespace units
}  // namespace game

#endif  // AAIRACE_SOURCE_GAME_UNITS_CITY_CAR_H_
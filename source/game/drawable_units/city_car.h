#ifndef AAIRACE_SOURCE_CITY_CAR_H_
#define AAIRACE_SOURCE_CITY_CAR_H_

#include <SFML/Graphics.hpp>

#include "car.h"

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
  virtual void Update(float elapsed_time) override;

  // Car
  virtual tools::Rectangle GetIntersectRectangle() const override;
 private:
  static sf::Texture texture_;
  static float kScaleCarImage;

  const float kDefaultSpeed = 0.25f;
  const float kMaxSpeed = kDefaultSpeed + 0.1f;
  const float kAccelerateValue = 0.00001f;
  const float kDefaultSlowDown = 0.00006f;
};

#endif  // AAIRACE_SOURCE_CITY_CAR_H_
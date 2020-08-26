#ifndef AAIRACE_SOURCE_CAR_H_
#define AAIRACE_SOURCE_CAR_H_

#include <SFML/Graphics.hpp>

#include "drawable_unit.h"
#include "tools/tools.h"

class Car : public DrawableUnit {
 public:
  Car(int x, int y, const DrawFunction& draw_function);
  virtual ~Car() {}

  virtual void SetHeroCar(const std::shared_ptr<Car>& hero_car);

  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time) override;
  
  virtual void Push(float dx, float dy);

  virtual float x() const;
  virtual float y() const;
  virtual float speed() const;

  virtual tools::Rectangle GetIntersectRectangle() const = 0;

 protected:
  const float kPushAccelerateX = 0.2f;
  const float kPushAccelerateY = 0.04f;
  const float kPushSlowDownAccelerate = 0.0004f;
  
  sf::Sprite sprite_;

  float speed_;

  float sprite_x_;
  float sprite_y_;

  float push_speed_x_;
  float push_speed_y_;
  float push_dx_;
  float push_dy_;

  std::shared_ptr<Car> hero_car_;
};

#endif  // AAIRACE_SOURCE_CAR_H_
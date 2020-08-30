#ifndef AAIRACE_SOURCE_GAME_UNITS_CAR_H_
#define AAIRACE_SOURCE_GAME_UNITS_CAR_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>
#include <common/tools.h>

namespace game {
namespace units {
class Car : public common::DrawableUnit {
 public:
  Car() = delete;
  Car(const Car& car) = delete;
  Car(Car&& car) = delete;
  Car& operator=(const Car& car) = delete;
  Car& operator=(Car&& car) = delete;

  Car(int x, int y, const DrawFunction& draw_function);
  virtual ~Car() {}

  virtual void SetHeroCar(const std::shared_ptr<Car>& hero_car);

  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;

  virtual void Push(float dx, float dy);
  virtual float x() const;
  virtual float y() const;
  virtual float speed() const;
  
  virtual common::Rectangle GetIntersectRectangle() const = 0;

  void SetBlockMove(bool is_blocked);
 protected:
  sf::Sprite sprite_;
  float speed_;
  float sprite_x_;
  float sprite_y_;
  float push_speed_x_;
  float push_speed_y_;
  float push_dx_;
  float push_dy_;
  bool is_move_blocked_;
  std::shared_ptr<Car> hero_car_;
};
}  // namespace units
}  // namespace game

#endif  // AAIRACE_SOURCE_GAME_UNITS_CAR_H_

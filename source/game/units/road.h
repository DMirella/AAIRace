#ifndef AAIRACE_SOURCE_GAME_UNITS_ROAD_H_
#define AAIRACE_SOURCE_GAME_UNITS_ROAD_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

namespace game {
namespace units {

class RacingCar;
class Road : public common::DrawableUnit {
 public:
  Road() = delete;
  Road(const Road& road) = delete;
  Road(Road&& road) = delete;
  Road&& operator=(const Road& road) = delete;
  Road&& operator=(Road&& road) = delete;

  Road(int screen_width, int screen_height, const DrawFunction& draw_function);

  void SetHeroCar(const std::shared_ptr<RacingCar>& hero_car);

  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;

  float left_x() const;
  float right_x() const;
  float width() const;
  float finish_line_sprite_y() const;
  float start_line_sprite_y() const;
 private:
  sf::Texture road_texture_;
  sf::Texture start_finish_line_texture_;

  sf::Sprite sprite1_;
  sf::Sprite sprite2_;
  sf::Sprite start_line_sprite_;
  sf::Sprite finish_line_sprite_;
  
  int screen_width_;
  int screen_height_;
  
  float sprite1_y_;
  float sprite2_y_;
  float start_line_sprite_y_;
  float finish_line_sprite_y_;
  float start_finish_line_x_;
  float speed_;

  std::shared_ptr<RacingCar> hero_car_;
};
}  // namespace units
}  // namespace game

#endif  // AAIRACE_SOURCE_GAME_UNITS_ROAD_H_
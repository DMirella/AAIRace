#ifndef AAIRACE_SOURCE_RACING_PROGRESS_BAR_H_
#define AAIRACE_SOURCE_RACING_PROGRESS_BAR_H_

#include <memory>
#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

namespace game {
namespace units {
class Road;
class RacingCar;
class RacingProgressBar : public common::DrawableUnit {
 public:
  RacingProgressBar() = delete;
  RacingProgressBar(const RacingProgressBar& bar) = delete;
  RacingProgressBar(RacingProgressBar&& bar) = delete;
  RacingProgressBar& operator=(const RacingProgressBar& bar) = delete;
  RacingProgressBar& operator=(RacingProgressBar&& bar) = delete;

  RacingProgressBar(int x, int y, int width, int height,
                    const DrawFunction& draw_function,
                    const std::shared_ptr<Road>& road,
                    const std::shared_ptr<RacingCar>& hero_car,
                    const std::vector<std::shared_ptr<RacingCar>>& enemy_cars);
  
  static void Init();

  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time,
                      const ui::UserControllersContext& context) override;
 private:
  static sf::Texture racing_car_icon_texture_;

  int x_;
  int y_;
  int width_;
  int height_; 

  sf::RectangleShape fill_progress_rectangle_;
  std::shared_ptr<Road> road_;
  std::shared_ptr<RacingCar> hero_car_;
  sf::Sprite hero_car_icon_sprite_;
  std::vector<std::pair<std::shared_ptr<RacingCar>, sf::Sprite>> enemy_cars_;
};
}  // namespace units
}  // namespace game

#endif  // AAIRACE_SOURCE_RACING_PROGRESS_BAR_H_

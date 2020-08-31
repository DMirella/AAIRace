#include "racing_progress_bar.h"

#include "road.h"
#include "racing_car.h"

namespace {
sf::Color gBasicRectColor = sf::Color(0xFC, 0xF8, 0xF8);
}  // namespace

namespace game {
namespace units {
sf::Texture RacingProgressBar::racing_car_icon_texture_ = sf::Texture();

RacingProgressBar::RacingProgressBar(int x, int y, int width, int height,
                                     const DrawFunction& draw_function,
                                     const std::shared_ptr<Road>& road,
                                     const std::shared_ptr<RacingCar>& hero_car,
                                     const std::vector<std::shared_ptr<RacingCar>>& enemy_cars)
    : DrawableUnit(draw_function)
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
    , road_(road)
    , hero_car_(hero_car) {
  fill_progress_rectangle_.setPosition(sf::Vector2f(x_, y_));
  fill_progress_rectangle_.setSize(sf::Vector2f(width, height));
  fill_progress_rectangle_.setFillColor(gBasicRectColor);
  fill_progress_rectangle_.setOutlineColor(sf::Color::Black);
  fill_progress_rectangle_.setOutlineThickness(1);
  hero_car_icon_sprite_.setPosition(x, y + height);
  hero_car_icon_sprite_.setTexture(racing_car_icon_texture_);
  hero_car_icon_sprite_.setColor(sf::Color::Red);
  for (const auto& it:  enemy_cars) {
    sf::Sprite sprite;
    sprite.setTexture(racing_car_icon_texture_);
    sprite.setColor(sf::Color::Blue);
    enemy_cars_.emplace_back(std::make_pair(it, sprite));
  }
}

void RacingProgressBar::Init() {
  const std::string kCarImageFile = "resources/small_racing_car.png";
  sf::Image car_image;
  car_image.loadFromFile(kCarImageFile);
  racing_car_icon_texture_.loadFromImage(car_image);
}

void RacingProgressBar::Update(float elapsed_time, const ui::UserControllersContext& context) {
  float start_finish_distance = road_->start_line_sprite_y() - road_->finish_line_sprite_y();
  for (auto& it : enemy_cars_) {
    float current_distance_from_finish = it.first->y() - road_->finish_line_sprite_y();
    float current_left_pass_road_percent = current_distance_from_finish / start_finish_distance;
    it.second.setPosition(
        x_, y_ + current_left_pass_road_percent * (height_ - racing_car_icon_texture_.getSize().y));
  }
  float current_distance_from_finish = hero_car_->y() - road_->finish_line_sprite_y();
  float current_left_pass_road_percent = current_distance_from_finish / start_finish_distance;
  hero_car_icon_sprite_.setPosition(
      x_, y_ + current_left_pass_road_percent * (height_ - racing_car_icon_texture_.getSize().y));
}

void RacingProgressBar::Draw() {
  draw_function_(fill_progress_rectangle_);
  for (const auto& it : enemy_cars_) {
    draw_function_(it.second);
  }
  draw_function_(hero_car_icon_sprite_);
}

}  // namespace units
}  // namespace game
#include "city_car.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {
const float gDefaultSpeed = 0.25f;
const float gMaxSpeed = gDefaultSpeed + 0.1f;
const float gAccelerateValue = 0.00001f;
const float gDefaultSlowDown = 0.00006f;
}  // namespace

namespace game {
namespace units {
  
sf::Texture CityCar::texture_ = sf::Texture();

CityCar::CityCar(int x, int y,  const DrawFunction& draw_function) 
    : Car(x, y, draw_function) {
  sprite_.setTexture(texture_);
  sprite_.setOrigin(texture_.getSize().x / 2, texture_.getSize().y / 2);
  sprite_.setPosition(sprite_x_, sprite_y_);
}

void CityCar::Init() {
  const std::string kCarImageFile = "resources/city_car.png";
  sf::Image car_image;
  car_image.loadFromFile(kCarImageFile);
  texture_.loadFromImage(car_image);
}

float CityCar::height() {
  return texture_.getSize().y;
}

void CityCar::Update(float elapsed_time, const ui::UserControllersContext& context) {
  Car::Update(elapsed_time, context);
  if (speed_ > gDefaultSpeed) {
    speed_ = std::min(speed_, gMaxSpeed);
    speed_ -= gDefaultSlowDown * elapsed_time;
    speed_ = std::max(speed_, gDefaultSpeed);
  }
  if (speed_ < gDefaultSpeed) {
    speed_ = std::max(speed_, 0.0f);
    speed_ += gAccelerateValue * elapsed_time;
    speed_ = std::min(speed_, gDefaultSpeed);
  }
  if (is_move_blocked_) {
    speed_ = 0;
  }
  sprite_.setPosition(sprite_x_, sprite_y_);
}

common::Rectangle CityCar::GetIntersectRectangle() const {
  const float kOffsetFromBorder = 8.0f;
  common::Rectangle result;
  result.y1 = sprite_y_ - texture_.getSize().y / 2.0f + kOffsetFromBorder;
  result.y2 = sprite_y_ + texture_.getSize().y / 2.0f - kOffsetFromBorder;
  result.x1 = sprite_x_ - texture_.getSize().x / 2.0f + kOffsetFromBorder;
  result.x2 = sprite_x_ + texture_.getSize().x / 2.0f - kOffsetFromBorder;
  return result;
}

}  // namespace units
}  // namespace game
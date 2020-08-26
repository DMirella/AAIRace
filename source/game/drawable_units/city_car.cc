#include "city_car.h"

#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>

#include "racing_car.h"

sf::Texture CityCar::texture_ = sf::Texture();

CityCar::CityCar(int x, int y,  const DrawFunction& draw_function) 
    : Car(x, y, draw_function) {
  sprite_.setTexture(texture_);
  sprite_.setOrigin(texture_.getSize().x / 2, texture_.getSize().y / 2);
  sprite_.setPosition(sprite_x_, sprite_y_);
}

void CityCar::Init() {
  const std::string kCarImageFile = "images/city_car.png";
  sf::Image car_image;
  car_image.loadFromFile(kCarImageFile);
  texture_.loadFromImage(car_image);
}

float CityCar::height() {
  return texture_.getSize().y;
}

void CityCar::Update(float elapsed_time) {
  Car::Update(elapsed_time);

  if (speed_ > kDefaultSpeed) {
    speed_ = std::min(speed_, kMaxSpeed);
    speed_ -= kDefaultSlowDown * elapsed_time;
    speed_ = std::max(speed_, kDefaultSpeed);
  }

  if (speed_ < kDefaultSpeed) {
    speed_ = std::max(speed_, 0.0f);
    speed_ += kAccelerateValue * elapsed_time;
    speed_ = std::min(speed_, kDefaultSpeed);
  }

  sprite_.setPosition(sprite_x_, sprite_y_);
}

tools::Rectangle CityCar::GetIntersectRectangle() const {
  const float kOffsetFromBorder = 8.0f;
  tools::Rectangle result;
  result.y1 = sprite_y_ - texture_.getSize().y / 2.0f + kOffsetFromBorder;
  result.y2 = sprite_y_ + texture_.getSize().y / 2.0f - kOffsetFromBorder;
  result.x1 = sprite_x_ - texture_.getSize().x / 2.0f + kOffsetFromBorder;
  result.x2 = sprite_x_ + texture_.getSize().x / 2.0f - kOffsetFromBorder;
  return result;
}

#include "racing_car.h"

#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>

sf::Texture RacingCar::texture_ = sf::Texture();

namespace {
const float gDefaultSpeed = 0.3f;
const float gAccelerateValue = 0.00001f;
const float gSlowDownValue = 0.0001f;
const float gDefaultSlowDown = 0.00006f;
const float gMaxCarSpeed = gDefaultSpeed + 0.2f;
const float gMinCarSpeed = gDefaultSpeed - 0.15f;
const float gTurnSpeed = 0.1f;
}  // namespace

RacingCar::RacingCar(int x, int y, const DrawFunction& draw_function) 
    : Car(x, y, draw_function)
    , is_car_accelerate_now_(false)
    , is_car_slow_down_now_(false)
    , is_car_turn_left_now_(false)
    , is_car_turn_right_now_(false) {
  sprite_.setTexture(texture_);
  sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
  sprite_.setPosition(sprite_x_, sprite_y_);
}

void RacingCar::Init() {
  const std::string kCarImageFile = "resources/racing_car.png";
	sf::Image car_image;
  car_image.loadFromFile(kCarImageFile);
  texture_.loadFromImage(car_image);
}

float RacingCar::height() {
  return texture_.getSize().y;
}

void RacingCar::Accelerate() {
  if (!is_move_blocked_) {
    is_car_accelerate_now_ = true;
  }
}

void RacingCar::SlowDown() {
  if (!is_move_blocked_) {
    is_car_slow_down_now_ = true;
  }
}

void RacingCar::TurnLeft() {
  if (!is_move_blocked_) {
    is_car_turn_left_now_ = true;
  }
}

void RacingCar::TurnRight() {
  if (!is_move_blocked_) {
    is_car_turn_right_now_ = true;
  }
}

void RacingCar::Update(float elapsed_time, const UserControllersContext& context) {
  Car::Update(elapsed_time, context);

  if (is_car_accelerate_now_ && speed_ < gMaxCarSpeed) {
    speed_ += gAccelerateValue * elapsed_time;
    speed_ = std::min(speed_, gMaxCarSpeed);
  } else {
    if (!is_move_blocked_ && speed_ > gDefaultSpeed) {
      speed_ = std::min(speed_, gMaxCarSpeed);
      speed_ -= gDefaultSlowDown * elapsed_time;
      speed_ = std::max(speed_, gDefaultSpeed);
    } else if (is_move_blocked_ && speed_ > 0) {
      speed_ -= gDefaultSlowDown * elapsed_time;
      speed_ = std::max(speed_, 0.0f);
    }
  }

  if (is_car_slow_down_now_ && speed_ > gMinCarSpeed) {
    speed_ -= gSlowDownValue*elapsed_time;
    speed_ = std::max(speed_, gMinCarSpeed);
  } else {
    if (!is_move_blocked_ && speed_ < gDefaultSpeed) {
      speed_ = std::max(speed_, 0.0f);
      speed_ += gAccelerateValue * elapsed_time;
      speed_ = std::min(speed_, gDefaultSpeed);
    }
  }

  float dx = 0;
  if (is_car_turn_right_now_) {
    dx += gTurnSpeed * elapsed_time;
  }

  if (is_car_turn_left_now_) {
    dx += -gTurnSpeed * elapsed_time;
  }

  sprite_x_ += dx;

  sprite_.setPosition(sprite_x_, sprite_y_);
  // sprite_.move(std::sin(angle_) * speed_, std::cos(angle_) * -speed_);
  // sprite_.setRotation(180 / 3.14 * angle_);
}

void RacingCar::Draw() {
  Car::Draw();
  ResetControls();
}

void RacingCar::SetHeroCar(const std::shared_ptr<Car>& hero_car) {
  Car::SetHeroCar(hero_car);
  if (this == hero_car_.get()) {
    sprite_.setColor(sf::Color::Red);
  } else {
    sprite_.setColor(sf::Color::Blue);
  }
}

tools::Rectangle RacingCar::GetIntersectRectangle() const {
  const float kOffsetFromBorder = 8.0f;
  tools::Rectangle result;
  result.y1 = sprite_y_ - texture_.getSize().y / 2.0f + kOffsetFromBorder;
  result.y2 = sprite_y_ + texture_.getSize().y / 2.0f - kOffsetFromBorder;
  result.x1 = sprite_x_ - texture_.getSize().x / 2.0f + kOffsetFromBorder;
  result.x2 = sprite_x_ + texture_.getSize().x / 2.0f - kOffsetFromBorder;
  return result;
}

void RacingCar::GetControllersState(bool* const is_car_accelerates,
                                    bool* const is_car_slows_down,
                                    bool* const is_car_turns_left,
                                    bool* const is_car_turns_right) {
  *is_car_accelerates = is_car_accelerate_now_;
  *is_car_slows_down = is_car_slow_down_now_;
  *is_car_turns_left = is_car_turn_left_now_;
  *is_car_turns_right = is_car_turn_right_now_;
}

float RacingCar::max_speed() {
  return gMaxCarSpeed;
}

void RacingCar::ResetControls() {
  is_car_accelerate_now_ = false;
  is_car_slow_down_now_ = false;
  is_car_turn_left_now_ = false;
  is_car_turn_right_now_ = false;
}

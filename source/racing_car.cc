#include "racing_car.h"

#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>

sf::Texture RacingCar::texture_ = sf::Texture();

RacingCar::RacingCar(int x, int y, const DrawFunction& draw_function) 
    : Car(x, y, draw_function)
    , is_car_accelerate_now_(false)
    , is_car_slow_down_now_(false)
    , is_car_turn_left_now_(false)
    , is_car_turn_right_now_(false)
    , is_move_blocked_(false) {
  sprite_.setTexture(texture_);
  sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
  sprite_.setPosition(sprite_x_, sprite_y_);
}

void RacingCar::Init() {
  const std::string kCarImageFile = "images/racing_car.png";
	sf::Image car_image;
  car_image.loadFromFile(kCarImageFile);
  texture_.loadFromImage(car_image);
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

void RacingCar::SetBlockMove(bool is_blocked) {
  is_move_blocked_ = is_blocked;
}

void RacingCar::Update(float elapsed_time) {
  Car::Update(elapsed_time);

  if (is_car_accelerate_now_ && speed_ < kMaxCarSpeed) {
    speed_ += kAccelerateValue * elapsed_time;
    speed_ = std::min(speed_, kMaxCarSpeed);
  } else {
    if (!is_move_blocked_ && speed_ > kDefaultSpeed) {
      speed_ = std::min(speed_, kMaxCarSpeed);
      speed_ -= kDefaultSlowDown * elapsed_time;
      speed_ = std::max(speed_, kDefaultSpeed);
    } else if (is_move_blocked_ && speed_ > 0) {
      speed_ -= kDefaultSlowDown * elapsed_time;
      speed_ = std::max(speed_, 0.0f);
    }
  }

  if (is_car_slow_down_now_ && speed_ > kMinCarSpeed) {
    speed_ -= kSlowDownValue*elapsed_time;
    speed_ = std::max(speed_, kMinCarSpeed);
  } else {
    if (!is_move_blocked_ && speed_ < kDefaultSpeed) {
      speed_ = std::max(speed_, 0.0f);
      speed_ += kAccelerateValue * elapsed_time;
      speed_ = std::min(speed_, kDefaultSpeed);
    }
  }

  float dx = 0;
  if (is_car_turn_right_now_) {
    dx += kTurnSpeed * elapsed_time;
  }

  if (is_car_turn_left_now_) {
    dx += -kTurnSpeed * elapsed_time;
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

Rectangle RacingCar::GetIntersectRectangle() const {
  const float kOffsetFromBorder = 8.0f;
  Rectangle result;
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

void RacingCar::ResetControls() {
  is_car_accelerate_now_ = false;
  is_car_slow_down_now_ = false;
  is_car_turn_left_now_ = false;
  is_car_turn_right_now_ = false;
}

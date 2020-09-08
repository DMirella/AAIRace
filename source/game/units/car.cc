#include "car.h"

#include "racing_car.h"

namespace {
const float gEpsilon = 1e-6;
const float gPushAccelerateX = 0.2f;
const float gPushAccelerateY = 0.04f;
const float gPushSlowDownAccelerate = 0.0004f;
}  // namespace

namespace game {
namespace units {

Car::Car(int x, int y, const DrawFunction& draw_function) 
    : DrawableUnit(draw_function)
    , sprite_(sf::Sprite())
    , speed_(0.0f)
    , sprite_x_(static_cast<float>(x))
    , sprite_y_(static_cast<float>(y))
    , push_speed_x_(0.0f)
    , push_speed_y_(0.0f)
    , push_dx_(0.0f)
    , push_dy_(0.0f)
    , is_move_blocked_(false)
    , hero_car_(nullptr) {
}

void Car::SetX(float x) {
  sprite_x_ = x;
}

void Car::SetY(float y) {
  sprite_y_ = y;
}

void Car::SetHeroCar(const std::shared_ptr<Car>& hero_car) {
  hero_car_ = hero_car;
}

void Car::Draw() {
  draw_function_(sprite_);
}

void Car::Update(float elapsed_time, const ui::UserControllersContext& context) {
  (void) context;
  speed_ -= push_dy_ * push_speed_y_ * elapsed_time;
  sprite_x_ += push_dx_ * push_speed_x_ * elapsed_time;

  if (this != hero_car_.get() && nullptr != hero_car_) {
    sprite_y_ += (hero_car_->speed() - speed_) * elapsed_time;
  }

  push_speed_x_ -= gPushSlowDownAccelerate * elapsed_time;
  push_speed_y_ -= gPushSlowDownAccelerate * elapsed_time;
  push_speed_x_ = std::max(push_speed_x_, 0.0f);
  push_speed_y_ = std::max(push_speed_y_, 0.0f);
}

void Car::Push(float dx, float dy) {
  if (push_speed_x_ < gEpsilon) {
    push_dx_ = dx;
    push_speed_x_ = gPushAccelerateX;
  }
  if (push_speed_y_ < gEpsilon) {
    push_dy_ = dy;
    push_speed_y_ = gPushAccelerateY;
  }
}

float Car::x() const {
  return sprite_x_;
}

float Car::y() const {
  return sprite_y_;
}

float Car::speed() const {
  return speed_;
}

void Car::SetBlockMove(bool is_blocked) {
  is_move_blocked_ = is_blocked;
}

}  // namespace units
}  // namespace game
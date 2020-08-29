#include "road.h"

#include <iostream>

#include "racing_car.h"

namespace {
const float gRoadX = 298.0f;
}  // namespace

Road::Road(int screen_width, int screen_height, const DrawFunction& draw_function) 
  : DrawableUnit(draw_function)
  , screen_width_(screen_width)
  , screen_height_(screen_height)
  , speed_(0.0f) {
  const std::string kRoadTextureFile = "resources/road.jpg";
  const std::string kStartFinishLineTextureFile = "resources/finish_line.png";

  sf::Image road_image, start_finish_line_image;
  road_image.loadFromFile(kRoadTextureFile);
  start_finish_line_image.loadFromFile(kStartFinishLineTextureFile);
  
  const float kScaleStartFinishLineX = 0.537f;
  const float kScaleStartFinishLineY = 0.5f;
  const float kStartLineY = 450.0f;
  const float kFinishLineY = -100000.0f;
  const float kFinishLineXOffset = 34;
  // const float kFinishLineY = -1000.0f;

  road_texture_.loadFromImage(road_image);
  start_finish_line_texture_.loadFromImage(start_finish_line_image);

  sprite1_.setTexture(road_texture_);
  sprite2_.setTexture(road_texture_);

  start_finish_line_x_ = gRoadX + kFinishLineXOffset;
  start_line_sprite_y_ = kStartLineY;
  finish_line_sprite_y_ = kFinishLineY;

  start_line_sprite_.setTexture(start_finish_line_texture_);
  start_line_sprite_.scale(kScaleStartFinishLineX, kScaleStartFinishLineY);
  start_line_sprite_.setPosition(start_finish_line_x_, start_line_sprite_y_);

  finish_line_sprite_.setTexture(start_finish_line_texture_);
  finish_line_sprite_.scale(kScaleStartFinishLineX, kScaleStartFinishLineY);
  finish_line_sprite_.setPosition(start_finish_line_x_, finish_line_sprite_y_);
  
  sprite1_y_ = -screen_height_;
  sprite2_y_ = 0;

  sprite1_.setPosition(0, sprite1_y_);
  sprite2_.setPosition(0, sprite2_y_);
}

void Road::SetHeroCar(const std::shared_ptr<RacingCar>& hero_car) {
  hero_car_ = hero_car;
}


void Road::Draw() {
  draw_function_(sprite1_);
  draw_function_(sprite2_);
  draw_function_(start_line_sprite_);
  draw_function_(finish_line_sprite_);
}

void Road::Update(float elapsed_time, const UserControllersContext& context) {
  (void) context;
  
  speed_ = hero_car_->speed();
  sprite1_y_ += speed_ * elapsed_time;
  sprite2_y_ += speed_ * elapsed_time;
  start_line_sprite_y_ += speed_ * elapsed_time;
  finish_line_sprite_y_ += speed_ * elapsed_time;
  
  if (sprite1_y_ > 0) {
    sprite1_y_ = -screen_height_;
    sprite2_y_ = 0;
  }

  sprite1_.setPosition(0, sprite1_y_);
  sprite2_.setPosition(0, sprite2_y_);
  start_line_sprite_.setPosition(start_finish_line_x_, start_line_sprite_y_);
  finish_line_sprite_.setPosition(start_finish_line_x_, finish_line_sprite_y_);
}

float Road::left_x() const {
  return gRoadX;
}

float Road::right_x() const {
  return sprite1_.getTexture()->getSize().x - gRoadX;
}

float Road::width() const {
  return right_x() - left_x();
}

float Road::finish_line_sprite_y() const {
  return finish_line_sprite_y_;
}

float Road::start_line_sprite_y() const {
  return start_line_sprite_y_;
}
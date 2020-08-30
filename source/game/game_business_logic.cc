#include "game_business_logic.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <ai/enemy_ai.h>
#include <ui/game_window.h>
#include <common/tools.h>

#include "drawable_units/car.h"
#include "drawable_units/city_car.h"
#include "drawable_units/racing_car.h"

namespace {
const float gSecondsCountBeforeStart = 3.0f;
const float gOneSecondInLocalElapsedTime = 5000.0f;
} // namespace 


GameBusinessLogic::GameBusinessLogic(const GameWindowContext& game_window_context, int enemies_count, 
                                     int city_car_count, const std::vector<std::shared_ptr<EnemyAI>>& enemies_ai)
  : game_window_context_(game_window_context)
  , start_timer_(0.0f)
  , finish_timer_(0.0f)
  , hero_racing_car_place_(1)
  , is_game_ended_(false)
  , is_hero_car_reached_finish_(false)
  , is_racing_started_(false)
  , enemies_ai_(enemies_ai) {
  assert(enemies_ai.size() == enemies_count);
  
  is_enemy_racing_car_finished_.resize(enemies_count, false);

  road_ = std::make_unique<Road>(game_window_context_.screen_width, game_window_context_.screen_height, game_window_context_.draw_function);

  const int kInfoLabelHeight = game_window_context_.screen_height / 5;
  const int kInfoLabelOutlineBorder = 5;
  info_label_ = std::make_unique<CenterAlignLabel>(tools::Rectangle(road_->left_x(), 0, road_->right_x(), kInfoLabelHeight),
                                                   "", kInfoLabelHeight, game_window_context_.draw_function);
  info_label_->SetOutlineBorder(kInfoLabelOutlineBorder);

  float kStartRacingCarY = road_->start_line_sprite_y() + 130.0f;

  float car_offset = (road_->right_x() - road_->left_x()) / (enemies_count + 1);

  float current_racing_car_x = road_->left_x() + car_offset / 2, previous_racing_car_x;
  int hero_car_start_place = std::rand() % (enemies_count + 1);
  hero_racing_car_ = std::make_shared<RacingCar>(current_racing_car_x + car_offset * hero_car_start_place, 
                                                 kStartRacingCarY, game_window_context_.draw_function);
  hero_racing_car_->SetHeroCar(hero_racing_car_);
  car_list_.push_back(hero_racing_car_);
  racing_car_list_.push_back(hero_racing_car_);

  road_->SetHeroCar(hero_racing_car_);

  for (int i = 0; i < enemies_count; i++) {
    if (i == hero_car_start_place) {
      previous_racing_car_x = current_racing_car_x;
      current_racing_car_x = previous_racing_car_x + car_offset;
    }
    
    std::shared_ptr<RacingCar> current_car = std::make_shared<RacingCar>(current_racing_car_x, kStartRacingCarY, game_window_context_.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    racing_car_list_.push_back(current_car);
    enemies_car_list_.push_back(current_car);
    car_list_.push_back(current_car);

    previous_racing_car_x = current_racing_car_x;
    current_racing_car_x = previous_racing_car_x + car_offset;
  }
  for (int i = 0; i < city_car_count; i++) {
    const float kStartYCityCarFromStartLineOffset = 1000.0f;
    auto current_x = tools::RandomFloat(road_->left_x(), road_->right_x());
    auto current_y = tools::RandomFloat(road_->finish_line_sprite_y(), 
                                        road_->start_line_sprite_y() - kStartYCityCarFromStartLineOffset);
    std::shared_ptr<CityCar> current_car = std::make_shared<CityCar>(current_x, current_y, game_window_context_.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    city_car_list_.push_back(current_car);
    car_list_.push_back(current_car);
  }

  for (auto& it : car_list_) {
    it->SetBlockMove(true);
  }
}

void GameBusinessLogic::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  ProcessStartGame(elapsed_time);
  ProcessEndGame(elapsed_time);
  CheckHeroControllers(context);
  MakeEnemiesTurn();
  ProcessGameEvents();
  Update(elapsed_time, context);
}

void GameBusinessLogic::ProcessStartGame(float elapsed_time) {
  if (!is_hero_car_reached_finish_ && !is_racing_started_) {
    start_timer_ += elapsed_time;
    for (int i = 1; i <= gSecondsCountBeforeStart; i++) {
      if (start_timer_ <= i * gOneSecondInLocalElapsedTime) {
        info_label_->SetText(std::to_string(i));
        return;
      }
    }
    if (start_timer_ <= (gSecondsCountBeforeStart + 1) * gOneSecondInLocalElapsedTime) {
      const std::string kStartText = "START!";
      info_label_->SetText(kStartText);
      return;
    } else {
      is_racing_started_ = true;
      for (auto& it : car_list_) {
        it->SetBlockMove(false);
      }
      info_label_->SetText("");
    }
  }
}

void GameBusinessLogic::ProcessEndGame(float elapsed_time) {
  if (is_racing_started_ && is_hero_car_reached_finish_) {
    finish_timer_ += elapsed_time;
    if (finish_timer_ <= 2 * gOneSecondInLocalElapsedTime) {
      const std::string kPlaceInfoText = std::to_string(hero_racing_car_place_) + "st place!";
      info_label_->SetText(kPlaceInfoText);
    } else {
      is_game_ended_ = true;
      info_label_->SetText("");
    }
  }
}

void GameBusinessLogic::CheckHeroControllers(const UserControllersContext& context) {
  if (context.is_left_arrow_pressed) { 
    hero_racing_car_->TurnLeft(); 
  }
  if (context.is_right_arrow_pressed) { 
    hero_racing_car_->TurnRight(); 
  }
  if (context.is_up_arrow_pressed) { 
    hero_racing_car_->Accelerate(); 
  }
  if (context.is_down_arrow_pressed) {
    hero_racing_car_->SlowDown(); 
  }
}

void GameBusinessLogic::MakeEnemiesTurn() {
  for (int i = 0; i < enemies_car_list_.size(); i++) {
    const auto& current_enemy = enemies_car_list_[i];
    AIOutputData ai_output_data = enemies_ai_[i]->GetOutputData(GetAIInputDataRegardingToRacingCar(current_enemy));
    if (ai_output_data.is_accelerate_action_active) {
      current_enemy->Accelerate();
    }
    if (ai_output_data.is_car_slow_action_active) {
      current_enemy->SlowDown();
    }
    if (ai_output_data.is_car_turn_left_action_active) {
      current_enemy->TurnLeft();
    }
    if (ai_output_data.is_car_turn_right_action_active) {
      current_enemy->TurnRight();
    }
  }
}

void GameBusinessLogic::ProcessGameEvents() {
  for (int i = 0; i < enemies_car_list_.size(); i++) {
    if (enemies_car_list_[i]->GetIntersectRectangle().y1 <= road_->finish_line_sprite_y()) {
      enemies_car_list_[i]->SetBlockMove(true);
      if (!is_enemy_racing_car_finished_[i] && !is_hero_car_reached_finish_) {
        hero_racing_car_place_++;
        is_enemy_racing_car_finished_[i] = true;
      }
    }
  }
  if (hero_racing_car_->GetIntersectRectangle().y1 <= road_->finish_line_sprite_y()) {
    is_hero_car_reached_finish_ = true;
    hero_racing_car_->SetBlockMove(true);
  }

  for (int i = 0; i < car_list_.size(); i++) {
    auto& first_car = car_list_[i];
    if (first_car->GetIntersectRectangle().x1 < road_->left_x()) {
      car_list_[i]->Push(1, 0);
    }
    if (first_car->GetIntersectRectangle().x2 > road_->right_x()) {
      car_list_[i]->Push(-1, 0);
    }
    for (int j = 0; j < car_list_.size(); j++) {
      if (i != j) {
        auto& second_car = car_list_[j];
        
        if (tools::CheckRectangleIntersect(first_car->GetIntersectRectangle(), second_car->GetIntersectRectangle())) {
          float dx = first_car->x() - second_car->x(), dy = first_car->y() - second_car->y();
          float length = std::sqrt(dx * dx + dy * dy);
          dx /= length;
          dy /= length;
          dy *= std::fabs(second_car->speed() - first_car->speed());
          first_car->Push(dx, dy);
          second_car->Push(-dx, -dy);
        }
      }
    }
  }
}

void GameBusinessLogic::Update(float elapsed_time, const UserControllersContext& context) {
  road_->Update(elapsed_time, context);
  for (auto& it : car_list_) {
    it->Update(elapsed_time, context);
  }
}

void GameBusinessLogic::DrawEntities() {
  road_->Draw();
  for (const auto& it : car_list_) {
    it->Draw();
  }
  info_label_->Draw();
  // DrawSensors(hero_racing_car_);
}

bool GameBusinessLogic::is_game_ended() const {
  return is_game_ended_;
}

AIIOData GameBusinessLogic::GetAIIODataRegardingToHeroCar() const {
  auto input_data = GetAIInputDataRegardingToRacingCar(hero_racing_car_);

  AIOutputData output_data;
  hero_racing_car_->GetControllersState(&output_data.is_accelerate_action_active,
                                        &output_data.is_car_slow_action_active,
                                        &output_data.is_car_turn_left_action_active,
                                        &output_data.is_car_turn_right_action_active);

  AIIOData result;
  result.input = std::move(input_data);
  result.output = std::move(output_data);
  return result;
}

AIInputData GameBusinessLogic::GetAIInputDataRegardingToRacingCar(const std::shared_ptr<RacingCar>& racing_car) const {
  const float kMaxDistance = 2.0f * game_window_context_.screen_height;
  const float kMaxSpeedInGame = RacingCar::max_speed();
  const float k2PI = 2.0f * tools::gPI;

  AIInputData input_data;
  input_data.distance_to_left_border = (racing_car->x() - road_->left_x()) / road_->width();
  input_data.distance_to_right_border = (road_->right_x() - racing_car->x()) / road_->width();
  input_data.current_speed = racing_car->speed() / kMaxSpeedInGame;

  const float kAngleIncreaseValue = k2PI / AIInputData::kCountDistanceScanRays;
  int i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue, i++) {
    AIInputData::OtherCarRegardingCurrentCarInfo car_info;
    car_info.distance = kMaxDistance;
    car_info.speed = kMaxSpeedInGame;
    tools::Point origin_point(racing_car->x(), racing_car->y());
    tools::Point ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : racing_car_list_) {
      if (it != racing_car) {
        auto current_distance = tools::GetDistanceFromRayToReactangle(origin_point, ray_direction, it->GetIntersectRectangle());
        if (car_info.distance > current_distance) {
          car_info.distance = current_distance;
          car_info.speed = it->speed();
        }
      }
    }
    car_info.distance /= kMaxDistance;
    car_info.speed /= kMaxSpeedInGame;
    input_data.distance_to_racing_cars[i] = std::move(car_info);
  }
  
  i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue, i++) {
    AIInputData::OtherCarRegardingCurrentCarInfo car_info;
    car_info.distance = kMaxDistance;
    car_info.speed = kMaxSpeedInGame;
    tools::Point origin_point(racing_car->x(), racing_car->y());
    tools::Point ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : city_car_list_) {
      auto current_distance = tools::GetDistanceFromRayToReactangle(origin_point, ray_direction, it->GetIntersectRectangle());
      if (car_info.distance > current_distance) {
        car_info.distance = current_distance;
        car_info.speed = it->speed();
      }
    }
    car_info.distance /= kMaxDistance;
    car_info.speed /= kMaxSpeedInGame;
    input_data.distance_to_city_cars[i] = std::move(car_info);
  }

  return input_data;
}

void GameBusinessLogic::DrawSensors(const std::shared_ptr<RacingCar>& car) const {
  const float kMaxDistance = 2.0f * game_window_context_.screen_height;
  const float kMaxSpeedInGame = RacingCar::max_speed();
  const float k2PI = 2.0f * tools::gPI;

  const float kAngleIncreaseValue = k2PI / AIInputData::kCountDistanceScanRays;
  int i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue, i++) {
    float current_minimum_distance = kMaxDistance;
    tools::Point ray_origin = tools::Point(car->x(), car->y()), ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : racing_car_list_) {
      if (it != car) {
        auto current_distance = tools::GetDistanceFromRayToReactangle(ray_origin, ray_direction, it->GetIntersectRectangle());
        if (current_minimum_distance > current_distance) {
          current_minimum_distance = current_distance;
        }
      }
    }
    if (current_minimum_distance < kMaxDistance) {
      sf::VertexArray lines(sf::LinesStrip, 2);
      lines[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * 300, ray_origin.y + ray_direction.y * 300);
      game_window_context_.draw_function(lines);
      
      sf::VertexArray lines_2(sf::LinesStrip, 2);
      lines_2[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines_2[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * current_minimum_distance, 
                                        ray_origin.y + ray_direction.y * current_minimum_distance);
      lines_2[0].color = sf::Color::Red;
      lines_2[1].color = sf::Color::Red;
      game_window_context_.draw_function(lines_2);
    }
  }
  
  i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue, i++) {
    float current_minimum_distance = kMaxDistance;
    tools::Point ray_origin = tools::Point(car->x(), car->y()), ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : city_car_list_) {
      auto current_distance = tools::GetDistanceFromRayToReactangle(ray_origin, ray_direction, it->GetIntersectRectangle());
      if (current_minimum_distance > current_distance) {
        current_minimum_distance = current_distance;
      }
    }
    if (current_minimum_distance < kMaxDistance) {
      sf::VertexArray lines(sf::LinesStrip, 2);
      lines[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * 300, ray_origin.y + ray_direction.y * 300);
      game_window_context_.draw_function(lines);
      
      sf::VertexArray lines_2(sf::LinesStrip, 2);
      lines_2[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines_2[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * current_minimum_distance, 
                                        ray_origin.y + ray_direction.y * current_minimum_distance);
      lines_2[0].color = sf::Color::Red;
      lines_2[1].color = sf::Color::Red;
      game_window_context_.draw_function(lines_2);
    }
  }
}
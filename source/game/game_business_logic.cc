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

#include "units/car.h"
#include "units/city_car.h"
#include "units/racing_car.h"

namespace {
const float gSecondsCountBeforeStart = 3.0f;
const float gOneSecondInLocalElapsedTime = 5000.0f;
} // namespace 

namespace game {
GameBusinessLogic::GameBusinessLogic(const ui::GameWindowContext& game_window_context, 
                                     int enemies_count, int city_car_count, 
                                     const std::vector<std::shared_ptr<ai::EnemyAI>>& enemies_ai)
  : game_window_context_(game_window_context)
  , start_timer_(0.0f)
  , finish_timer_(0.0f)
  , hero_racing_car_place_(1)
  , is_game_ended_(false)
  , is_hero_car_reached_finish_(false)
  , is_racing_started_(false)
  , enemies_ai_(enemies_ai) {
  assert(enemies_ai.size() == enemies_count);

  faced_car_grid_.resize(enemies_count + 1 + city_car_count, std::vector<bool>(enemies_count + 1 + city_car_count));
  
  is_enemy_racing_car_finished_.resize(enemies_count, false);

  road_ = std::make_shared<units::Road>(
      game_window_context_.screen_width,game_window_context_.screen_height, 
      game_window_context_.draw_function);

  const int kInfoLabelHeight = game_window_context_.screen_height / 5;
  const int kInfoLabelOutlineBorder = 5;
  info_label_ = std::make_unique<ui::CenterAlignLabel>(
      common::Rectangle(road_->left_x(), 0, road_->right_x(), kInfoLabelHeight),
      "", kInfoLabelHeight, game_window_context_.draw_function);
  info_label_->SetOutlineBorder(kInfoLabelOutlineBorder);

  const int kHeroPlaceLabelHeight = game_window_context_.screen_height / 7;
  const int kHeroPlaceOutlineBorder = 1;
  hero_place_label_ = std::make_unique<ui::CenterAlignLabel>(
      common::Rectangle(road_->right_x(), 0,
                        game_window_context_.screen_width, kHeroPlaceLabelHeight),
      "", kHeroPlaceLabelHeight, game_window_context_.draw_function);
  // hero_place_label_->SetOutlineBorder(kHeroPlaceOutlineBorder);

  float kStartRacingCarY = road_->start_line_sprite_y() + 130.0f;
  float car_offset = (road_->right_x() - road_->left_x()) / (enemies_count + 1);
  float current_racing_car_x = road_->left_x() + car_offset / 2, previous_racing_car_x;
  int hero_car_start_place = std::rand() % (enemies_count + 1);
  hero_racing_car_ = std::make_shared<units::RacingCar>(
      current_racing_car_x + car_offset * hero_car_start_place, 
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
    
    std::shared_ptr<units::RacingCar> current_car = std::make_shared<units::RacingCar>(
        current_racing_car_x, kStartRacingCarY, game_window_context_.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    racing_car_list_.push_back(current_car);
    enemies_car_list_.push_back(current_car);
    car_list_.push_back(current_car);

    previous_racing_car_x = current_racing_car_x;
    current_racing_car_x = previous_racing_car_x + car_offset;
  }
  for (int i = 0; i < city_car_count; i++) {
    const float kStartYCityCarFromStartLineOffset = 1000.0f;
    auto current_x = common::RandomFloat(road_->left_x(), road_->right_x());
    auto current_y = common::RandomFloat(
        road_->finish_line_sprite_y(), 
        road_->start_line_sprite_y() - kStartYCityCarFromStartLineOffset);
    std::shared_ptr<units::CityCar> current_car = std::make_shared<units::CityCar>(
        current_x, current_y, game_window_context_.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    city_car_list_.push_back(current_car);
    car_list_.push_back(current_car);
  }

  const int kProgressBarWidth = 20;
  const int kProgressBarHeight = game_window_context_.screen_height / 1.2f;
  const int kProgressBarX = 40;
  const int kProgressBarY = (game_window_context_.screen_height - kProgressBarHeight) / 2;
  racing_progress_bar_ = std::make_unique<units::RacingProgressBar>(
      kProgressBarX, kProgressBarY, kProgressBarWidth, kProgressBarHeight,
      game_window_context_.draw_function, road_, hero_racing_car_,
      enemies_car_list_);

  for (auto& it : car_list_) {
    it->SetBlockMove(true);
  }
}

void GameBusinessLogic::NotifyGameCycleElapsed(float elapsed_time,
                                               const ui::UserControllersContext& context) {
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

void GameBusinessLogic::CheckHeroControllers(const ui::UserControllersContext& context) {
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
    ai::AIOutputData ai_output_data 
        = enemies_ai_[i]->GetOutputData(GetAIInputDataRegardingToRacingCar(current_enemy));
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
        const bool cars_intersect 
            = common::CheckRectangleIntersect(first_car->GetIntersectRectangle(), second_car->GetIntersectRectangle());
        if (cars_intersect) {
          faced_car_grid_[i][j] = true;
          faced_car_grid_[j][i] = true;
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

void GameBusinessLogic::Update(float elapsed_time, const ui::UserControllersContext& context) {
  int count_above_hero_car = 0;
  for (const auto& it : enemies_car_list_) {
    if (it->y() < hero_racing_car_->y()) {
      count_above_hero_car++;
    }
  }
  hero_place_label_->SetText(std::to_string(count_above_hero_car + 1) + "/"
                                + std::to_string(enemies_car_list_.size() + 1));
  road_->Update(elapsed_time, context);
  for (auto& it : car_list_) {
    it->Update(elapsed_time, context);
  }
  racing_progress_bar_->Update(elapsed_time, context);
}

void GameBusinessLogic::DrawEntities() {
  road_->Draw();
  for (const auto& it : car_list_) {
    it->Draw();
  }
  info_label_->Draw();
  if (is_racing_started_) {
    hero_place_label_->Draw();
    racing_progress_bar_->Draw();
  }
  // DrawSensors(hero_racing_car_);
}

bool GameBusinessLogic::is_game_ended() const {
  return is_game_ended_;
}

ai::AIIOData GameBusinessLogic::GetAIIODataRegardingToHeroCar() const {
  auto input_data = GetAIInputDataRegardingToRacingCar(hero_racing_car_);

  ai::AIOutputData output_data;
  hero_racing_car_->GetControllersState(&output_data.is_accelerate_action_active,
                                        &output_data.is_car_slow_action_active,
                                        &output_data.is_car_turn_left_action_active,
                                        &output_data.is_car_turn_right_action_active);

  ai::AIIOData result;
  result.input = std::move(input_data);
  result.output = std::move(output_data);
  return result;
}

ai::AIInputData GameBusinessLogic::GetAIInputDataRegardingToRacingCar(
    const std::shared_ptr<units::RacingCar>& racing_car) const {
  const float kMaxDistance = 2.0f * game_window_context_.screen_height;
  const float kMaxSpeedInGame = units::RacingCar::max_speed();
  const float k2PI = 2.0f * common::gPI;

  ai::AIInputData input_data;
  input_data.distance_to_left_border = (racing_car->x() - road_->left_x()) / road_->width();
  input_data.distance_to_right_border = (road_->right_x() - racing_car->x()) / road_->width();
  input_data.current_speed = racing_car->speed() / kMaxSpeedInGame;

  int racing_car_place_in_face_grid = 0;
  for (; racing_car_place_in_face_grid < faced_car_grid_.size(); racing_car_place_in_face_grid++) {
    if (car_list_[racing_car_place_in_face_grid] == racing_car) {
      break;
    }
  }

  const float kAngleIncreaseValue = k2PI / ai::AIInputData::kCountDistanceScanRays;
  int i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue) {
    ai::AIInputData::OtherCarRegardingCurrentCarInfo car_info;
    car_info.distance = kMaxDistance;
    car_info.speed = kMaxSpeedInGame;
    std::shared_ptr<units::Car> current_nearest_car = nullptr;
    common::Point origin_point(racing_car->x(), racing_car->y());
    common::Point ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : racing_car_list_) {
      if (it != racing_car) {
        auto current_distance = common::GetDistanceFromRayToReactangle(
            origin_point, ray_direction, it->GetIntersectRectangle());
        if (car_info.distance > current_distance) {
          car_info.distance = current_distance;
          car_info.speed = it->speed();
          current_nearest_car = it;
        }
      }
    }
    car_info.distance /= kMaxDistance;
    car_info.speed /= kMaxSpeedInGame;
    for (int i = 0; i < faced_car_grid_[racing_car_place_in_face_grid].size(); i++) {
      if (car_list_[i] == current_nearest_car) {
        car_info.faced = faced_car_grid_[racing_car_place_in_face_grid][i];
        break;
      }
    }
    input_data.distance_to_racing_cars[i] = std::move(car_info);
    i++;
  }
  
  i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue) {
    ai::AIInputData::OtherCarRegardingCurrentCarInfo car_info;
    car_info.distance = kMaxDistance;
    car_info.speed = kMaxSpeedInGame;
    std::shared_ptr<units::Car> current_nearest_car = nullptr;
    common::Point origin_point(racing_car->x(), racing_car->y());
    common::Point ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : city_car_list_) {
      auto current_distance = common::GetDistanceFromRayToReactangle(
          origin_point, ray_direction, it->GetIntersectRectangle());
      if (car_info.distance > current_distance) {
        car_info.distance = current_distance;
        car_info.speed = it->speed();
        current_nearest_car = it;
      }
    }
    car_info.distance /= kMaxDistance;
    car_info.speed /= kMaxSpeedInGame;
    for (int i = 0; i < faced_car_grid_[racing_car_place_in_face_grid].size(); i++) {
      if (car_list_[i] == current_nearest_car) {
        car_info.faced = faced_car_grid_[racing_car_place_in_face_grid][i];
        break;
      }
    }
    input_data.distance_to_city_cars[i] = std::move(car_info);
    i++;
  }

  return input_data;
}

void GameBusinessLogic::DrawSensors(const std::shared_ptr<units::RacingCar>& car) const {
  const float kMaxDistance = 2.0f * game_window_context_.screen_height;
  const float kMaxSpeedInGame = units::RacingCar::max_speed();
  const float k2PI = 2.0f * common::gPI;

  int racing_car_place_in_face_grid = 0;
  for (; racing_car_place_in_face_grid < faced_car_grid_.size(); racing_car_place_in_face_grid++) {
    if (car_list_[racing_car_place_in_face_grid] == car) {
      break;
    }
  }

  const float kAngleIncreaseValue = k2PI / ai::AIInputData::kCountDistanceScanRays;
  int i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue) {
    std::shared_ptr<units::Car> current_nearest_car = nullptr;
    float current_minimum_distance = kMaxDistance;
    common::Point ray_origin = common::Point(
        car->x(), car->y()), ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : racing_car_list_) {
      if (it != car) {
        auto current_distance = common::GetDistanceFromRayToReactangle(
            ray_origin, ray_direction, it->GetIntersectRectangle());
        if (current_minimum_distance > current_distance) {
          current_minimum_distance = current_distance;
          current_nearest_car = it;
        }
      }
    }
    bool is_faced = false;
    for (int i = 0; i < faced_car_grid_[racing_car_place_in_face_grid].size(); i++) {
      if (car_list_[i] == current_nearest_car) {
        is_faced = faced_car_grid_[racing_car_place_in_face_grid][i];
        break;
      }
    }
    if (current_minimum_distance < kMaxDistance) {
      sf::VertexArray lines(sf::LinesStrip, 2);
      lines[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * 300, 
                                       ray_origin.y + ray_direction.y * 300);
      game_window_context_.draw_function(lines);
      
      sf::VertexArray lines_2(sf::LinesStrip, 2);
      lines_2[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines_2[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * current_minimum_distance, 
                                         ray_origin.y + ray_direction.y * current_minimum_distance);
      if (is_faced) {
        lines_2[0].color = sf::Color::Red;
        lines_2[1].color = sf::Color::Red;
      } else {
        lines_2[0].color = sf::Color::Yellow;
        lines_2[1].color = sf::Color::Yellow;
      }
      game_window_context_.draw_function(lines_2);
    }
    i++;
  }
  
  i = 0;
  for (float current_angle = 0.0f; current_angle <= k2PI; current_angle += kAngleIncreaseValue) {
    std::shared_ptr<units::Car> current_nearest_car = nullptr;
    float current_minimum_distance = kMaxDistance;
    common::Point ray_origin = common::Point(
        car->x(), car->y()), ray_direction(std::cos(current_angle), std::sin(current_angle));
    for (const auto& it : city_car_list_) {
      auto current_distance = common::GetDistanceFromRayToReactangle(
          ray_origin, ray_direction, it->GetIntersectRectangle());
      if (current_minimum_distance > current_distance) {
        current_minimum_distance = current_distance;
        current_nearest_car = it;
      }
    }
    if (current_minimum_distance < kMaxDistance) {
      sf::VertexArray lines(sf::LinesStrip, 2);
      lines[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * 300,
                                       ray_origin.y + ray_direction.y * 300);
      game_window_context_.draw_function(lines);
      
      sf::VertexArray lines_2(sf::LinesStrip, 2);
      lines_2[0].position = sf::Vector2f(ray_origin.x, ray_origin.y);
      lines_2[1].position = sf::Vector2f(ray_origin.x + ray_direction.x * current_minimum_distance, 
                                         ray_origin.y + ray_direction.y * current_minimum_distance);
      bool is_faced = false;
      for (int i = 0; i < faced_car_grid_[racing_car_place_in_face_grid].size(); i++) {
        if (car_list_[i] == current_nearest_car) {
          is_faced = faced_car_grid_[racing_car_place_in_face_grid][i];
          break;
        }
      }
      if (is_faced) {
        lines_2[0].color = sf::Color::Red;
        lines_2[1].color = sf::Color::Red;
      } else {
        lines_2[0].color = sf::Color::Yellow;
        lines_2[1].color = sf::Color::Yellow;
      }
      game_window_context_.draw_function(lines_2);
    }
    i++;
  }
}
}  // namespace game
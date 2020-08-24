#include "game_business_logic.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "city_car.h"
#include "game_window.h"
#include "tools.h"

GameBusinessLogic::GameBusinessLogic(const GameWindowContext& game_window_context, int enemies_count, int city_car_count, const std::vector<EnemyAI>& enemies_ai)
  : is_game_session_ended_(false)
  , enemies_ai_(enemies_ai) {
  assert(enemies_ai.size() == enemies_count);
  
  RacingCar::Init();
  CityCar::Init();

  road_ = std::make_shared<Road>(game_window_context.screen_width, game_window_context.screen_height, game_window_context.draw_function);
  float kStartRacingCarY = road_->start_line_sprite_y() + 90.0f;

  float car_offset = (road_->right_x() - road_->left_x()) / (enemies_count + 1);

  float current_racing_car_x = road_->left_x() + car_offset / 2, previous_racing_car_x;
  hero_racing_car_ = std::make_shared<RacingCar>(current_racing_car_x, kStartRacingCarY, game_window_context.draw_function);
  hero_racing_car_->SetHeroCar(hero_racing_car_);
  car_list_.push_back(hero_racing_car_);
  racing_car_list_.push_back(hero_racing_car_);

  road_->SetHeroCar(hero_racing_car_);

  for (int i = 0; i < enemies_count; i++) {
    previous_racing_car_x = current_racing_car_x;
    current_racing_car_x = previous_racing_car_x + car_offset;
    std::shared_ptr<RacingCar> current_car = std::make_shared<RacingCar>(current_racing_car_x, kStartRacingCarY, game_window_context.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    racing_car_list_.push_back(current_car);
    enemies_car_list_.push_back(current_car);
    car_list_.push_back(current_car);
  }
  for (int i = 0; i < city_car_count; i++) {
    const float kStartYCityCarFromStartLineOffset = 1000.0f;
    auto current_x = RandomFloat(road_->left_x(), road_->right_x());
    auto current_y = RandomFloat(road_->finish_line_sprite_y(), 
                                 road_->start_line_sprite_y() - kStartYCityCarFromStartLineOffset);
    std::shared_ptr<CityCar> current_car = std::make_shared<CityCar>(current_x, current_y, game_window_context.draw_function);
    current_car->SetHeroCar(hero_racing_car_);
    car_list_.push_back(current_car);
  }
}

void GameBusinessLogic::NotifyGameCycleElapsed(float elapsed_time) {
  CheckHeroControllers();
  MakeEnemiesTurn();
  ProcessGameEvents();
  Update(elapsed_time);
}

void GameBusinessLogic::CheckHeroControllers() {
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) { 
    hero_racing_car_->TurnLeft(); 
  }
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) { 
    hero_racing_car_->TurnRight(); 
  }
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) { 
    hero_racing_car_->Accelerate(); 
  }
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {
    hero_racing_car_->SlowDown(); 
  }
}

void GameBusinessLogic::MakeEnemiesTurn() {
  for (int i = 0; i < enemies_car_list_.size(); i++) {
    const auto& current_enemy = enemies_car_list_[i];
    AIOutputData ai_output_data = enemies_ai_[i].GetOutputData(GetAIInputDataRegardingToRacingCar(current_enemy));
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
  for (int i = 0; i < racing_car_list_.size(); i++) {
    if (racing_car_list_[i]->GetIntersectRectangle().y1 <= road_->finish_line_sprite_y()) {
      racing_car_list_[i]->SetBlockMove(true);
      is_game_session_ended_ = true;
    }
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
        
        if (CheckRectangleIntersect(first_car->GetIntersectRectangle(), second_car->GetIntersectRectangle())) {
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

void GameBusinessLogic::Update(float elapsed_time) {
  road_->Update(elapsed_time);
  for (auto& it : car_list_) {
    it->Update(elapsed_time);
  }
}

void GameBusinessLogic::DrawEntities() {
  road_->Draw();
  for (const auto& it : car_list_) {
    it->Draw();
  }
}

bool GameBusinessLogic::is_game_session_ended() const {
  return is_game_session_ended_;
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

AIInputData GameBusinessLogic::GetAIInputDataRegardingToRacingCar(const std::shared_ptr<RacingCar>& car) const {
  AIInputData input_data;
  input_data.distance_to_left_border = car->x() - road_->left_x();
  input_data.distance_to_right_border = road_->right_x() - car->x();
  input_data.current_speed = car->speed();

  std::vector<AIInputData::OtherCarRegardingCurrentCarInfo> other_cars_info;
  for (const auto& current_car : car_list_) {
    if (current_car != car) {
      AIInputData::OtherCarRegardingCurrentCarInfo info;
      info.speed = current_car->speed();
      info.distance = EstimateDistance(car->x(), car->y(), current_car->x(), current_car->y());
      info.angle = std::atan((car->y() - current_car->y()) / (car->x() - current_car->x()));
      other_cars_info.emplace_back(info);
    }
  }

  std::sort(other_cars_info.begin(), other_cars_info.end(), 
      [](const AIInputData::OtherCarRegardingCurrentCarInfo& a, const AIInputData::OtherCarRegardingCurrentCarInfo& b) -> bool {
        return a.distance < b.distance;
      });
  for (int i = 0; i < AIInputData::kCountClosestMemorizedCars; i++) {
    input_data.closest_cars[i] = std::move(other_cars_info[i]);
  }
  return input_data;
}
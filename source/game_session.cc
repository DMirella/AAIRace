#include "game_session.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "city_car.h"
#include "tools.h"


GameSession::GameSession(const std::shared_ptr<sf::RenderWindow>& window, int enemies_count, int city_car_count) : window_(window) {
  RacingCar::Init();
  CityCar::Init();

  road_ = std::make_shared<Road>(window_->getSize().x, window_->getSize().y, window_);
  float kStartRacingCarY = road_->start_line_sprite_y() + 90.0f;

  float car_offset = (road_->right_x() - road_->left_x()) / (enemies_count + 1);

  float current_racing_car_x = road_->left_x() + car_offset / 2, previous_racing_car_x;
  hero_racing_car_ = std::make_shared<RacingCar>(current_racing_car_x, kStartRacingCarY, window_);
  hero_racing_car_->SetHeroCar(hero_racing_car_);
  car_list_.push_back(hero_racing_car_);
  racing_car_list_.push_back(hero_racing_car_);

  road_->SetHeroCar(hero_racing_car_);

  for (int i = 0; i < enemies_count; i++) {
    previous_racing_car_x = current_racing_car_x;
    current_racing_car_x = previous_racing_car_x + car_offset;
    std::shared_ptr<RacingCar> current_car = std::make_shared<RacingCar>(current_racing_car_x, kStartRacingCarY, window_);
    current_car->SetHeroCar(hero_racing_car_);
    racing_car_list_.push_back(current_car);
    car_list_.push_back(current_car);
  }
  for (int i = 0; i < city_car_count; i++) {
    const float kStartYCityCarFromStartLineOffset = 1000.0f;
    auto current_x = RandomFloat(road_->left_x(), road_->right_x());
    auto current_y = RandomFloat(road_->finish_line_sprite_y(), 
                                 road_->start_line_sprite_y() - kStartYCityCarFromStartLineOffset);
    std::shared_ptr<CityCar> current_car = std::make_shared<CityCar>(current_x, current_y, window_);
    current_car->SetHeroCar(hero_racing_car_);
    car_list_.push_back(current_car);
  }
}

void GameSession::CheckControllers() {
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

void GameSession::ProcessGameEvents() {
  for (int i = 0; i < racing_car_list_.size(); i++) {
    if (racing_car_list_[i]->GetIntersectRectangle().y1 <= road_->finish_line_sprite_y()) {
      racing_car_list_[i]->SetBlockMove(true);
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

void GameSession::Update(float elapsed_time) {
  road_->Update(elapsed_time);
  for (auto& it : car_list_) {
    it->Update(elapsed_time);
  }
}

void GameSession::DrawEntites() {
  road_->Draw();
  hero_racing_car_->Draw();
  for (const auto& it : car_list_) {
    it->Draw();
  }
}
#ifndef AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_
#define AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "ai/ai_types.h"
#include "drawable_units/road.h"
#include "ui/game_window.h"
#include "ui/center_align_label.h"

namespace sf {
class RenderWindow;
}  // namespace sf

class GameWindow;
class Car;
class CityCar;
class RacingCar;
class Road;
class EnemyAI;

class GameBusinessLogic {
 public:
  GameBusinessLogic() = delete;
  GameBusinessLogic(const GameBusinessLogic& game_business_logic) = delete;
  GameBusinessLogic(GameBusinessLogic&& game_business_logic) = delete;
  GameBusinessLogic& operator=(const GameBusinessLogic& game_business_logic) = delete;
  GameBusinessLogic& operator=(GameBusinessLogic&& game_business_logic) = delete;

  GameBusinessLogic(const GameWindowContext& game_window_context, int enemies_count, int city_car_count, 
                    const std::vector<std::shared_ptr<EnemyAI>>& enemies_ai);

  void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context);
  void DrawEntities();

  bool is_game_ended() const;

  AIIOData GetAIIODataRegardingToHeroCar() const;
 private:
  AIInputData GetAIInputDataRegardingToRacingCar(const std::shared_ptr<RacingCar>& car) const;
  void DrawSensors(const std::shared_ptr<RacingCar>& car) const;

  // Process game cycle
  void ProcessStartGame(float elapsed_time);
  void ProcessEndGame(float elapsed_time);
  void CheckHeroControllers();
  void MakeEnemiesTurn();
  void ProcessGameEvents();
  void Update(float elapsed_time, const UserControllersContext& context);

  GameWindowContext game_window_context_;

  float start_timer_;
  float finish_timer_;

  std::unique_ptr<CenterAlignLabel> info_label_;
  std::shared_ptr<RacingCar> hero_racing_car_;
  std::vector<std::shared_ptr<Car>> car_list_;
  std::vector<std::shared_ptr<CityCar>> city_car_list_;
  std::vector<std::shared_ptr<RacingCar>> racing_car_list_;
  std::vector<std::shared_ptr<RacingCar>> enemies_car_list_;
  std::vector<bool> is_enemy_racing_car_finished_;
  std::unique_ptr<Road> road_;

  std::vector<std::shared_ptr<EnemyAI>> enemies_ai_;

  int hero_racing_car_place_;

  bool is_racing_started_;
  bool is_hero_car_reached_finish_;
  bool is_game_ended_;
};

#endif  // AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_
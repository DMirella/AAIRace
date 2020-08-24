#ifndef AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_
#define AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_

#include <functional>
#include <memory>
#include <map>
#include <vector>

#include "ai_types.h"
#include "enemy_ai.h"
#include "game_window.h"
#include "racing_car.h"
#include "road.h"

namespace sf {
class RenderWindow;
}  // namespace sf

class GameWindow;

class GameBusinessLogic {
 public:
  GameBusinessLogic(const GameWindowContext& game_window_context, int enemies_count, int city_car_count, const std::vector<EnemyAI>& enemies_ai);

  void NotifyGameCycleElapsed(float elapsed_time);
  void DrawEntities();

  bool is_game_session_ended() const;

  AIIOData GetAIIODataRegardingToHeroCar() const;
 private:
  AIInputData GetAIInputDataRegardingToRacingCar(const std::shared_ptr<RacingCar>& car) const;

  void CheckHeroControllers();
  void MakeEnemiesTurn();
  void ProcessGameEvents();
  void Update(float elapsed_time);

  std::shared_ptr<RacingCar> hero_racing_car_;
  std::vector<std::shared_ptr<Car>> car_list_;
  std::vector<std::shared_ptr<RacingCar>> racing_car_list_;
  std::vector<std::shared_ptr<RacingCar>> enemies_car_list_;
  std::shared_ptr<Road> road_;

  std::vector<EnemyAI> enemies_ai_;

  bool is_game_session_ended_;
};

#endif  // AAIRACE_SOURCE_GAME_BUSINESS_LOGIC_H_
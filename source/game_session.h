#ifndef AAIRACE_SOURCE_GAME_SESSION_H_
#define AAIRACE_SOURCE_GAME_SESSION_H_

#include <memory>
#include <vector>

#include "racing_car.h"
#include "road.h"

namespace sf {
class RenderWindow;
}  // namespace sf

class GameSession {
 public:
  GameSession(const std::shared_ptr<sf::RenderWindow>&, int enemies_count, int city_car_count);

  void CheckControllers();
  void ProcessGameEvents();
  void Update(float elapsed_time);
  void DrawEntites();
 private:
  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<RacingCar> hero_racing_car_;
  std::vector<std::shared_ptr<Car>> car_list_;
  std::vector<std::shared_ptr<RacingCar>> racing_car_list_;
  std::shared_ptr<Road> road_;
};

#endif  // AAIRACE_SOURCE_GAME_SESSION_H_
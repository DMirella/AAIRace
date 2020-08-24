#ifndef AAIRACE_SOURCE_GAME_WINDOW_H_
#define AAIRACE_SOURCE_GAME_WINDOW_H_

#include <SFML/Graphics.hpp>

#include "drawable_unit.h"
#include "screen_state_machine.h"

struct GameWindowContext {
  int screen_width;
  int screen_height;
  DrawableUnit::DrawFunction draw_function;
};

// class ScreenStateMachine;
class GameWindow {
 public:
  GameWindow();

  void Start();
 private:
  void DrawSprite(const sf::Sprite& sprite);

  sf::RenderWindow window_;
  std::unique_ptr<ScreenStateMachine> screen_state_machine_;
};

#endif  // AAIRACE_SOURCE_GAME_WINDOW_H_
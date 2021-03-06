#ifndef AAIRACE_SOURCE_UI_GAME_WINDOW_H_
#define AAIRACE_SOURCE_UI_GAME_WINDOW_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

namespace ui {
struct GameWindowContext {
  int screen_width;
  int screen_height;
  common::DrawableUnit::DrawFunction draw_function;
};

class ScreenStateMachine;
class GameWindow {
 public:
  GameWindow(const GameWindow& game_window) = delete;
  GameWindow(GameWindow&& game_window) = delete;
  GameWindow& operator=(const GameWindow& game_window) = delete;
  GameWindow& operator=(GameWindow&& game_window) = delete;

  GameWindow();

  static void LoadGameTextures();
  void Start();
 private:
  void ExitGame();
  void Draw(const sf::Drawable& object);

  bool is_window_has_focus_;
  sf::RenderWindow window_;
  std::shared_ptr<ScreenStateMachine> screen_state_machine_;
};
}  // namespace ui

#endif  // AAIRACE_SOURCE_UI_GAME_WINDOW_H_

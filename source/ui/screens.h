#ifndef AAIRACE_SOURCE_SCREEN_H_
#define AAIRACE_SOURCE_SCREEN_H_

#include <memory>
#include <vector>

#include <game/level_manager.h>
#include "game_window.h"
#include "button.h"

class ScreenStateMachine;
class Screen {
 public:
  Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) {}
  virtual void Draw() {}
 protected:
  GameWindowContext game_window_context_;
  ScreenStateMachine* const screen_state_machine_;
};

class Button;
class MenuScreen : public Screen {
 public:
  MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) override;
  virtual void Draw() override;
 private:
  void OnStartGameButtonClick();
  void OnExitGameButtonClick();

  std::shared_ptr<Button> start_game_button_;
  std::shared_ptr<Button> exit_game_button_;
};

class LevelChooseScreen : public Screen {
 public:
  LevelChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) override;
  virtual void Draw() override;
 private:
  void OnLevelChoosen(int level);

  std::vector<std::shared_ptr<Button>> level_buttons_;
  std::vector<Button::OnClickCallback> level_button_callback_;
};

class GameSession;
class GameScreen : public Screen {
 public:
  GameScreen(ScreenStateMachine* const screen_state_machine, 
             const GameWindowContext& game_window_context,
             int game_level);

  virtual void NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) override;
  virtual void Draw() override;
 private:
  LevelManager level_manager_;
  std::shared_ptr<GameSession> active_game_session_;
};

#endif  // AAIRACE_SOURCE_SCREEN_H_
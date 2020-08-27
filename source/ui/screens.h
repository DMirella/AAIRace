#ifndef AAIRACE_SOURCE_SCREEN_H_
#define AAIRACE_SOURCE_SCREEN_H_

#include <memory>
#include <vector>

#include "button.h"
#include <game/level_manager.h>
#include "game_window.h"

class ScreenStateMachine;
class Screen {
 public:
  Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {}
  virtual void Draw() {}
 protected:
  GameWindowContext game_window_context_;
  ScreenStateMachine* const screen_state_machine_;
};

class ProfileChooseScreen : public Screen {
 public:
  ProfileChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context);
  virtual void Draw();
 private:
  void OnSignInButtonClick();
  void OnSignUpButtonClick();

  std::shared_ptr<Button> sign_in_button_;
  std::shared_ptr<Button> sign_up_button_;
};

class TextBox;
class SignUpScreen : public Screen {
 public:
  SignUpScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context);
  virtual void Draw();
 private:
  void OnRegisterButtonClick();

  std::shared_ptr<CenterAlignLabel> text_label_;
  std::shared_ptr<TextBox> name_text_box_;
  std::shared_ptr<Button> register_button_;
};

class SignInScreen : public Screen {
 public:
  SignInScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context);
  virtual void Draw();
 private:
  void OnLogInButtonClick();

  std::shared_ptr<CenterAlignLabel> text_label_;
  std::shared_ptr<TextBox> name_text_box_;
  std::shared_ptr<Button> register_button_;
};

class MenuScreen : public Screen {
 public:
  MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
 private:
  void OnStartGameButtonClick();
  void OnExitGameButtonClick();

  std::shared_ptr<Button> start_game_button_;
  std::shared_ptr<Button> exit_game_button_;
};

class CenterAlignLabel;
class LevelChooseScreen : public Screen {
 public:
  LevelChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
 private:
  void OnLevelChoosen(int level);

  std::shared_ptr<CenterAlignLabel> label_;
  std::vector<std::shared_ptr<Button>> level_buttons_;
  std::vector<Button::OnClickCallback> level_button_callback_;
};

class GameSession;
class GameScreen : public Screen {
 public:
  GameScreen(ScreenStateMachine* const screen_state_machine, 
             const GameWindowContext& game_window_context,
             int game_level);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
 private:
  LevelManager level_manager_;
  std::shared_ptr<GameSession> active_game_session_;
};

#endif  // AAIRACE_SOURCE_SCREEN_H_
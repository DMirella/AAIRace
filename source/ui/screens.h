#ifndef AAIRACE_SOURCE_SCREEN_H_
#define AAIRACE_SOURCE_SCREEN_H_

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include <game/level_manager.h>
#include "game_window.h"
#include "popup.h"

class ScreenStateMachine;
class Screen : public std::enable_shared_from_this<Screen> {
 public:
  Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {}
  virtual void Draw() {}
  virtual std::string GetScreenName() const = 0;
 protected:
  GameWindowContext game_window_context_;
  ScreenStateMachine* const screen_state_machine_;
};

class ProfileChooseScreen : public Screen {
 public:
  ProfileChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
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
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnRegisterButtonClick();
  void OnBackButtonClick();

  bool ok_button_error_popup_pressed_;
  std::unique_ptr<Button> back_button_;
  std::unique_ptr<Popup> active_popup_;
  std::shared_ptr<CenterAlignLabel> text_label_;
  std::shared_ptr<TextBox> name_text_box_;
  std::shared_ptr<Button> register_button_;
};

class SignInScreen : public Screen {
 public:
  SignInScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnLogInButtonClick();
  void OnBackButtonClick();

  bool ok_button_error_popup_pressed_;
  std::unique_ptr<Button> back_button_;
  std::unique_ptr<Popup> active_popup_;
  std::shared_ptr<CenterAlignLabel> text_label_;
  std::shared_ptr<TextBox> name_text_box_;
  std::shared_ptr<Button> register_button_;
};

class MenuScreen : public Screen {
 public:
  MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnStartGameButtonClick();
  void OnExitGameButtonClick();
  void OnLogOutButtonClick();

  std::unique_ptr<Button> log_out_button_;
  std::shared_ptr<Button> start_game_button_;
  std::shared_ptr<Button> exit_game_button_;
};

class CenterAlignLabel;
class LevelChooseScreen : public Screen {
 public:
  LevelChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnLevelChoosen(int level);
  void OnBackButtonClick();

  std::unique_ptr<Button> back_button_;
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
  virtual std::string GetScreenName() const override;
 private:
  std::shared_ptr<GameSession> active_game_session_;
};

class TransitionScreen : public Screen {
 public:
  TransitionScreen(ScreenStateMachine* const screen_state_machine, 
                   const GameWindowContext& game_window_context,
                   const std::shared_ptr<Screen>& current_screen,
                   const std::shared_ptr<Screen>& next_screen,
                   float transition_animation_time);
  virtual void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  float transition_animation_time_;
  float summary_transition_time_;
  float current_transperent_value_;
  sf::RectangleShape transition_rectangle_;
  sf::Color current_rectangle_color_;
  std::shared_ptr<Screen> current_screen_;
  std::shared_ptr<Screen> next_screen_;
};

#endif  // AAIRACE_SOURCE_SCREEN_H_
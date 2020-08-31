#ifndef AAIRACE_SOURCE_UI_SCREEN_H_
#define AAIRACE_SOURCE_UI_SCREEN_H_

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include <game/game_session.h>

#include "button.h"
#include "center_align_label.h"
#include "edit_text_box.h"
#include "game_window.h"
#include "image_popup.h"
#include "text_popup.h"

namespace ui {
class ScreenStateMachine;
class Screen : public std::enable_shared_from_this<Screen> {
 public:
  Screen() = delete;
  Screen(const Screen& screen) = delete;
  Screen(Screen&& screen) = delete;
  Screen& operator=(const Screen& screen) = delete;
  Screen& operator=(Screen&& screen) = delete;

  Screen(ScreenStateMachine* const screen_state_machine, 
         const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time, 
                                      const UserControllersContext& context) {}
  virtual void Draw() {}
  virtual std::string GetScreenName() const = 0;
 protected:
  GameWindowContext game_window_context_;
  ScreenStateMachine* const screen_state_machine_;
};

class ProfileChooseScreen : public Screen {
 public:
  ProfileChooseScreen() = delete;
  ProfileChooseScreen(const ProfileChooseScreen& profile_choose_screen) = delete;
  ProfileChooseScreen(ProfileChooseScreen&& profile_choose_screen) = delete;
  ProfileChooseScreen& operator=(const ProfileChooseScreen& profile_choose_screen) = delete;
  ProfileChooseScreen& operator=(ProfileChooseScreen&& profile_choose_screen) = delete;

  ProfileChooseScreen(ScreenStateMachine* const screen_state_machine, 
                      const GameWindowContext& game_window_context);
  virtual void NotifyGameCycleElapsed(float elapsed_time, 
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnSignInButtonClick();
  void OnSignUpButtonClick();

  std::unique_ptr<Button> sign_in_button_;
  std::unique_ptr<Button> sign_up_button_;
};

class UserRegistrationScreen : public Screen {
 public:
  UserRegistrationScreen() = delete;
  UserRegistrationScreen(const UserRegistrationScreen& user_registreation_screen) = delete;
  UserRegistrationScreen(UserRegistrationScreen&& user_registreation_screen) = delete;
  UserRegistrationScreen& operator=(const UserRegistrationScreen& user_registreation_screen) = delete;
  UserRegistrationScreen& operator=(UserRegistrationScreen&& user_registreation_screen) = delete;

  UserRegistrationScreen(ScreenStateMachine* const screen_state_machine,
                         const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnRegisterButtonClick();
  void OnBackButtonClick();

  bool ok_button_error_popup_pressed_;
  std::unique_ptr<Button> back_button_;
  std::unique_ptr<TextPopup> active_popup_;
  std::unique_ptr<CenterAlignLabel> text_label_;
  std::unique_ptr<EditTextBox> name_edit_text_box_;
  std::unique_ptr<Button> register_button_;
};

class UserLogInScreen : public Screen {
 public:
  UserLogInScreen() = delete;
  UserLogInScreen(const UserLogInScreen& user_login_screen) = delete;
  UserLogInScreen(UserLogInScreen&& user_login_screen) = delete;
  UserLogInScreen& operator=(const UserLogInScreen& user_login_screen) = delete;
  UserLogInScreen& operator=(UserLogInScreen&& user_login_screen) = delete;

  UserLogInScreen(ScreenStateMachine* const screen_state_machine,
                  const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnLogInButtonClick();
  void OnBackButtonClick();

  bool ok_button_error_popup_pressed_;
  std::unique_ptr<Button> back_button_;
  std::unique_ptr<TextPopup> active_popup_;
  std::unique_ptr<CenterAlignLabel> text_label_;
  std::unique_ptr<EditTextBox> name_edit_text_box_;
  std::unique_ptr<Button> register_button_;
};

class MenuScreen : public Screen {
 public:
  MenuScreen() = delete;
  MenuScreen(const MenuScreen& menu_screen) = delete;
  MenuScreen(MenuScreen&& menu_screen) = delete;
  MenuScreen& operator=(const MenuScreen& menu_screen) = delete;
  MenuScreen& operator=(MenuScreen&& menu_screen) = delete;

  MenuScreen(ScreenStateMachine* const screen_state_machine,
             const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnStartGameButtonClick();
  void OnTutorialGameButtonClick();
  void OnExitGameButtonClick();
  void OnLogOutButtonClick();

  bool ok_button_tutorial_popup_pressed_;
  std::unique_ptr<ImagePopup> tutorial_popup_;
  std::unique_ptr<Button> log_out_button_;
  std::unique_ptr<Button> start_game_button_;
  std::unique_ptr<Button> tutorial_game_button_;
  std::unique_ptr<Button> exit_game_button_;
};

class LevelChooseScreen : public Screen {
 public:
  LevelChooseScreen() = delete;
  LevelChooseScreen(const LevelChooseScreen& level_choose_screen) = delete;
  LevelChooseScreen(LevelChooseScreen&& level_choose_screen) = delete;
  LevelChooseScreen& operator=(const LevelChooseScreen& level_choose_screen) = delete;
  LevelChooseScreen& operator=(LevelChooseScreen&& level_choose_screen) = delete;

  LevelChooseScreen(ScreenStateMachine* const screen_state_machine,
                    const GameWindowContext& game_window_context);

  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  void OnLevelChoosen(int level);
  void OnBackButtonClick();

  std::unique_ptr<Button> back_button_;
  std::unique_ptr<CenterAlignLabel> label_;
  std::vector<std::unique_ptr<Button>> level_buttons_;
  std::vector<Button::OnClickCallback> level_button_callback_;
};

class GameScreen : public Screen {
 public:
  GameScreen() = delete;
  GameScreen(const GameScreen& game_screen) = delete;
  GameScreen(GameScreen&& game_screen) = delete;
  GameScreen& operator=(const GameScreen& game_screen) = delete;
  GameScreen& operator=(GameScreen&& game_screen) = delete;

  GameScreen(ScreenStateMachine* const screen_state_machine, 
             const GameWindowContext& game_window_context,
             int game_level);

  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
  virtual void Draw() override;
  virtual std::string GetScreenName() const override;
 private:
  std::unique_ptr<game::GameSession> active_game_session_;
};

class TransitionScreen : public Screen {
 public:
  TransitionScreen() = delete;
  TransitionScreen(const TransitionScreen& transition_screen) = delete;
  TransitionScreen(TransitionScreen&& transition_screen) = delete;
  TransitionScreen& operator=(const TransitionScreen& transition_screen) = delete;
  TransitionScreen& operator=(TransitionScreen&& transition_screen) = delete;

  TransitionScreen(ScreenStateMachine* const screen_state_machine, 
                   const GameWindowContext& game_window_context,
                   const std::shared_ptr<Screen>& current_screen,
                   const std::shared_ptr<Screen>& next_screen,
                   float transition_animation_time);
  virtual void NotifyGameCycleElapsed(float elapsed_time,
                                      const UserControllersContext& context) override;
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
}  // namespace ui

#endif  // AAIRACE_SOURCE_UI_SCREEN_H_
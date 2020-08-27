#include "screens.h"

#include <iostream>

#include "button.h"
#include "center_align_label.h"
#include "game/game_session.h"
#include "screen_state_machine.h"
#include "text_box.h"

// Screen
Screen::Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : game_window_context_(game_window_context)
    , screen_state_machine_(screen_state_machine) {
}

// ProfileChooseScreen
ProfileChooseScreen::ProfileChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context) 
    : Screen(screen_state_machine, game_window_context) {
  const int kButtonsCount = 2;
  const int kButtonYOffset = 10;
  const int kButtonsWidth = game_window_context_.screen_width / 3;
  const int kButtonsHeight = game_window_context_.screen_height / 8;
  const int kButtonsBlockStartX = game_window_context_.screen_width / 3;
  const int kButtonsBlockStartY = (game_window_context_.screen_height - kButtonsHeight - kButtonYOffset) / 2;
  const std::string kSignInButtonText = "SIGN IN";
  const std::string kSignUpButtonText = "SIGN UP";

  Button::OnClickCallback on_sign_in_button_click_callback = std::bind(&ProfileChooseScreen::OnSignInButtonClick, this); 
  Button::OnClickCallback on_sign_up_button_click_callback = std::bind(&ProfileChooseScreen::OnSignUpButtonClick, this);

  sign_in_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY, kButtonsWidth, kButtonsHeight, 
                                             kSignInButtonText, on_sign_in_button_click_callback, 
                                             game_window_context_.draw_function);
  sign_up_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY + kButtonsHeight + kButtonYOffset, kButtonsWidth, kButtonsHeight, 
                                             kSignUpButtonText, on_sign_up_button_click_callback, 
                                             game_window_context_.draw_function);
}

void ProfileChooseScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  sign_in_button_->Update(elapsed_time, context);
  sign_up_button_->Update(elapsed_time, context);
}

void ProfileChooseScreen::Draw() {
  sign_in_button_->Draw();
  sign_up_button_->Draw();
}

void ProfileChooseScreen::OnSignInButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<SignInScreen>(screen_state_machine_, game_window_context_));
}

void ProfileChooseScreen::OnSignUpButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<SignUpScreen>(screen_state_machine_, game_window_context_));
}

// SignUpScreen
SignUpScreen::SignUpScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kElementsYOffset = 10;
  const int kElementsCount = 3;
  const int kTextEditHeight = game_window_context_.screen_height / 8;
  const int kLabelHeight = game_window_context_.screen_height / 10;
  const int kElementsBlockStartX = game_window_context_.screen_width / 3;
  const int kElementsBlockWidth = game_window_context_.screen_width / 3;
  const int kRegisterButtonWidth = kElementsBlockWidth / 1.5f;
  const int kRegisterButtonHeight = kTextEditHeight / 1.2f;
  const int kElementsBlockStartY 
      = (game_window_context_.screen_height - kTextEditHeight - kLabelHeight - kRegisterButtonHeight - 2 * kElementsYOffset) / 2;
  const int kLabelFontSize = 50;
  const std::string kRegisterButtonText = "Create";
  const std::string kLabelText = "Enter your name:";

  text_label_ = std::make_shared<CenterAlignLabel>(tools::Rectangle(kElementsBlockStartX, kElementsBlockStartY, 
                                                                    kElementsBlockStartX + kElementsBlockWidth, kElementsBlockStartY + kLabelHeight),
                                                   kLabelText, kLabelFontSize, game_window_context_.draw_function);
  name_text_box_ = std::make_shared<TextBox>(kElementsBlockStartX, kElementsBlockStartY + kLabelHeight + kElementsYOffset, 
                                             kElementsBlockWidth, kTextEditHeight, game_window_context_.draw_function);
  Button::OnClickCallback on_register_button_click_callback = std::bind(&SignUpScreen::OnRegisterButtonClick, this); 
  register_button_ = std::make_shared<Button>(kElementsBlockStartX + (kElementsBlockWidth - kRegisterButtonWidth) / 2, 
                                              kElementsBlockStartY + kTextEditHeight + kLabelHeight + kElementsYOffset * 2, 
                                              kRegisterButtonWidth, kRegisterButtonHeight, 
                                              kRegisterButtonText, on_register_button_click_callback, 
                                              game_window_context_.draw_function);
}

void SignUpScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  text_label_->Update(elapsed_time, context);
  name_text_box_->Update(elapsed_time, context);
  register_button_->Update(elapsed_time, context);
}

void SignUpScreen::Draw() {
  text_label_->Draw();
  name_text_box_->Draw();
  register_button_->Draw();
}

void SignUpScreen::OnRegisterButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<MenuScreen>(screen_state_machine_, game_window_context_));
}

// SignInScreen
SignInScreen::SignInScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kElementsYOffset = 10;
  const int kElementsCount = 3;
  const int kTextEditHeight = game_window_context_.screen_height / 8;
  const int kLabelHeight = game_window_context_.screen_height / 10;
  const int kElementsBlockStartX = game_window_context_.screen_width / 3;
  const int kElementsBlockWidth = game_window_context_.screen_width / 3;
  const int kRegisterButtonWidth = kElementsBlockWidth / 1.5f;
  const int kRegisterButtonHeight = kTextEditHeight / 1.2f;
  const int kElementsBlockStartY 
      = (game_window_context_.screen_height - kTextEditHeight - kLabelHeight - kRegisterButtonHeight - 2 * kElementsYOffset) / 2;
  const int kLabelFontSize = 50;
  const std::string kRegisterButtonText = "GO!";
  const std::string kLabelText = "Enter your name:";

  text_label_ = std::make_shared<CenterAlignLabel>(tools::Rectangle(kElementsBlockStartX, kElementsBlockStartY, 
                                                                    kElementsBlockStartX + kElementsBlockWidth, kElementsBlockStartY + kLabelHeight),
                                                   kLabelText, kLabelFontSize, game_window_context_.draw_function);
  name_text_box_ = std::make_shared<TextBox>(kElementsBlockStartX, kElementsBlockStartY + kLabelHeight + kElementsYOffset, 
                                             kElementsBlockWidth, kTextEditHeight, game_window_context_.draw_function);
  Button::OnClickCallback on_register_button_click_callback = std::bind(&SignInScreen::OnLogInButtonClick, this); 
  register_button_ = std::make_shared<Button>(kElementsBlockStartX + (kElementsBlockWidth - kRegisterButtonWidth) / 2, 
                                              kElementsBlockStartY + kTextEditHeight + kLabelHeight + kElementsYOffset * 2, 
                                              kRegisterButtonWidth, kRegisterButtonHeight, 
                                              kRegisterButtonText, on_register_button_click_callback, 
                                              game_window_context_.draw_function);
}

void SignInScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  text_label_->Update(elapsed_time, context);
  name_text_box_->Update(elapsed_time, context);
  register_button_->Update(elapsed_time, context);
}

void SignInScreen::Draw() {
  text_label_->Draw();
  name_text_box_->Draw();
  register_button_->Draw();
}

void SignInScreen::OnLogInButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<MenuScreen>(screen_state_machine_, game_window_context_));
}

// MenuScreen
MenuScreen::MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kButtonsCount = 2;
  const int kButtonsWidth = game_window_context_.screen_width / 3;
  const int kButtonsHeight = game_window_context_.screen_height / 8;
  const int kButtonsBlockStartX = game_window_context_.screen_width / 3;
  const int kButtonsBlockStartY = (game_window_context_.screen_height - kButtonsHeight) / 2;
  const int kButtonYOffset = 10;
  const std::string kStartButtonText = "PLAY";
  const std::string kExitButtonText = "EXIT";

  Button::OnClickCallback on_start_game_button_click_callback = std::bind(&MenuScreen::OnStartGameButtonClick, this); 
  Button::OnClickCallback on_exit_game_button_click_callback = std::bind(&MenuScreen::OnExitGameButtonClick, this);

  start_game_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY, kButtonsWidth, kButtonsHeight, 
                                                kStartButtonText, on_start_game_button_click_callback, 
                                                game_window_context_.draw_function);
  exit_game_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY + kButtonsHeight + kButtonYOffset,
                                               kButtonsWidth, kButtonsHeight, kExitButtonText, on_exit_game_button_click_callback,
                                               game_window_context_.draw_function);
}

void MenuScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  start_game_button_->Update(elapsed_time, context);
  exit_game_button_->Update(elapsed_time, context);
}

void MenuScreen::Draw() {
  start_game_button_->Draw();
  exit_game_button_->Draw();
}

void MenuScreen::OnStartGameButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<LevelChooseScreen>(screen_state_machine_, game_window_context_));
}

void MenuScreen::OnExitGameButtonClick() {
  exit(0);
}

// LevelChooseScreen
LevelChooseScreen::LevelChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kLevelsCount = 30;
  const int kLevelButtonSize = 100;
  const int kLevelButtonXYOffset = 20;
  const int kLevelButtonsBlockXYOffset = 150;
  const int kMaxCountLevelButtonsX 
      = (game_window_context_.screen_width - 2 * kLevelButtonsBlockXYOffset) / (kLevelButtonSize + kLevelButtonXYOffset);
  const int kFontLabelSize = 50;
  const std::string kChooseLavelLabelText = "Choose level:";
    
  label_ = std::make_shared<CenterAlignLabel>(tools::Rectangle(0, 0, game_window_context_.screen_width, kLevelButtonsBlockXYOffset),
                                              kChooseLavelLabelText, kFontLabelSize, game_window_context_.draw_function);

  for (int current_level = 1; current_level <= kLevelsCount; current_level++) {
    Button::OnClickCallback on_current_level_button_click_callback = [this, current_level]() {
      OnLevelChoosen(current_level);
    };
    level_button_callback_.push_back(on_current_level_button_click_callback);
    level_buttons_.emplace_back(std::make_shared<Button>(
        kLevelButtonsBlockXYOffset + ((current_level - 1) % kMaxCountLevelButtonsX) * (kLevelButtonSize + kLevelButtonXYOffset),
        kLevelButtonsBlockXYOffset + ((current_level - 1) / kMaxCountLevelButtonsX) * (kLevelButtonSize + kLevelButtonXYOffset),
        kLevelButtonSize,
        kLevelButtonSize,
        std::to_string(current_level),
        on_current_level_button_click_callback,
        game_window_context_.draw_function
    ));
  }
}

void LevelChooseScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  for (const auto& it : level_buttons_) {
    it->Update(elapsed_time, context);
  }
}

void LevelChooseScreen::Draw() {
  label_->Draw();
  for (const auto& it : level_buttons_) {
    it->Draw();
  }
}

void LevelChooseScreen::OnLevelChoosen(int level) {
  screen_state_machine_->SetScreen(std::make_shared<GameScreen>(screen_state_machine_, game_window_context_, level));
}

// GameScreen
GameScreen::GameScreen(ScreenStateMachine* const screen_state_machine, 
                       const GameWindowContext& game_window_context,
                       int game_level)
    : Screen(screen_state_machine, game_window_context)
    , level_manager_(game_window_context)
    , active_game_session_(nullptr) {
  active_game_session_ = level_manager_.GenerateGameSession(game_level);
}

void GameScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  active_game_session_->NotifyGameCycleElapsed(elapsed_time, context);
}

void GameScreen::Draw() {
  if (!active_game_session_->is_game_session_ended()) {
    active_game_session_->DrawEntities();
  } else {
    screen_state_machine_->SetScreen(std::make_shared<LevelChooseScreen>(screen_state_machine_, game_window_context_));
  }
}
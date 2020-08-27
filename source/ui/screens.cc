#include "screens.h"

#include <iostream>

#include "button.h"
#include "game/game_session.h"
#include "screen_state_machine.h"
#include "center_align_label.h"

// Screen
Screen::Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : game_window_context_(game_window_context)
    , screen_state_machine_(screen_state_machine) {
}

// MenuScreen
MenuScreen::MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kButtonsCount = 2;
  const int kButtonsWidth = game_window_context.screen_width / 3;
  const int kButtonsHeight = game_window_context.screen_height / 8;
  const int kButtonsBlockStartX = game_window_context.screen_width / 3;
  const int kButtonsBlockStartY = (game_window_context.screen_height - kButtonsHeight) / 2;
  const int kButtonYOffset = 10;
  const std::string kStartButtonText = "PLAY";
  const std::string kExitButtonText = "EXIT";

  Button::OnClickCallback on_start_game_button_click_callback = std::bind(&MenuScreen::OnStartGameButtonClick, this); 
  Button::OnClickCallback on_exit_game_button_click_callback = std::bind(&MenuScreen::OnExitGameButtonClick, this);

  start_game_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY, kButtonsWidth, kButtonsHeight, 
                                                kStartButtonText, on_start_game_button_click_callback, 
                                                game_window_context.draw_function);
  exit_game_button_ = std::make_shared<Button>(kButtonsBlockStartX, kButtonsBlockStartY + kButtonsHeight + kButtonYOffset,
                                               kButtonsWidth, kButtonsHeight, kExitButtonText, on_exit_game_button_click_callback,
                                               game_window_context.draw_function);
}

void MenuScreen::NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) {
  start_game_button_->Update(elapsed_time, cursor_x, cursor_y, is_mouse_button_pressed);
  exit_game_button_->Update(elapsed_time, cursor_x, cursor_y, is_mouse_button_pressed);
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
      = (game_window_context.screen_width - 2 * kLevelButtonsBlockXYOffset) / (kLevelButtonSize + kLevelButtonXYOffset);
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
        game_window_context.draw_function
    ));
  }
}

void LevelChooseScreen::NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) {
  for (const auto& it : level_buttons_) {
    it->Update(elapsed_time, cursor_x, cursor_y, is_mouse_button_pressed);
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

void GameScreen::NotifyGameCycleElapsed(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) {
  active_game_session_->NotifyGameCycleElapsed(elapsed_time);
}

void GameScreen::Draw() {
  if (!active_game_session_->is_game_session_ended()) {
    active_game_session_->DrawEntities();
  } else {
    screen_state_machine_->SetScreen(std::make_shared<LevelChooseScreen>(screen_state_machine_, game_window_context_));
  }
}
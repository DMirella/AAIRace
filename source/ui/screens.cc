#include "screens.h"

#include <iostream>

#include "screen_state_machine.h"

Screen::Screen(ScreenStateMachine* const screen_state_machine)
    : screen_state_machine_(screen_state_machine) {

}

GameScreen::GameScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine)
    , active_internal_state_id_(InternalStateId::kLevelChoose)
    , active_game_session_(nullptr)
    , level_manager_(LevelManager(game_window_context)) {

}

void GameScreen::Close() {

}

void GameScreen::Show() {

}

void GameScreen::NotifyGameCycleElapsed(float elapsed_time) {
  if (InternalStateId::kLevelChoose == active_internal_state_id_) {
    active_game_session_ = std::make_unique<GameSession>(level_manager_.GenerateGameSession(1));
    active_internal_state_id_ = InternalStateId::kGameSessionActive;
  } else if (InternalStateId::kGameSessionActive == active_internal_state_id_) {
    active_game_session_->NotifyGameCycleElapsed(elapsed_time);
  }
}

void GameScreen::Draw() {
  if (InternalStateId::kGameSessionActive == active_internal_state_id_) {
    if (!active_game_session_->is_game_session_ended()) {
      active_game_session_->DrawEntities();
    } else {
      active_game_session_.reset();
      active_internal_state_id_ = InternalStateId::kLevelChoose;
    }
  }
}
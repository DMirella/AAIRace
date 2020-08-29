#include "screen_state_machine.h"

#include "game_window.h"
#include "screens.h"

ScreenStateMachine::ScreenStateMachine(const GameWindowContext& game_window_context, const std::function<void()>& exit_game_function)
    : exit_game_function_(exit_game_function)
    , active_screen_(std::make_shared<ProfileChooseScreen>(this, game_window_context))
    , active_user_profile_(game_window_context) {
}

ScreenStateMachine::~ScreenStateMachine() {
  active_user_profile_.SaveToConfigFile();
}

void ScreenStateMachine::SetScreen(const std::shared_ptr<Screen>& screen) {
  if (active_screen_->GetScreenName() != screen->GetScreenName()) {
    active_screen_ = screen;
  }
}

std::shared_ptr<Screen> ScreenStateMachine::active_screen() {
  return active_screen_;
}

UserProfile& ScreenStateMachine::GetUserProfile() {
  return active_user_profile_;
}

void ScreenStateMachine::ExitGame() {
  exit_game_function_();
}
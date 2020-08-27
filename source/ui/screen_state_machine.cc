#include "screen_state_machine.h"

#include "game_window.h"
#include "screens.h"

ScreenStateMachine::ScreenStateMachine(const GameWindowContext& game_window_context)
    : active_screen_(nullptr) {
  SetScreen(std::make_shared<ProfileChooseScreen>(this, game_window_context));
}

void ScreenStateMachine::SetScreen(const std::shared_ptr<Screen>& screen) {
  active_screen_ = screen;
}

std::shared_ptr<Screen> ScreenStateMachine::active_screen() {
  return active_screen_;
}
#include "screen_state_machine.h"

#include "game_window.h"
#include "screens.h"

ScreenStateMachine::ScreenStateMachine(const GameWindowContext& game_window_context)
    : active_screen_id_(ScreenId::kGameScreen) {
  screens_[ScreenId::kGameScreen] = std::make_shared<GameScreen>(this, game_window_context);

  SetScreen(ScreenId::kGameScreen);
}

void ScreenStateMachine::SetScreen(ScreenId screen_id) {
  if (screen_id != active_screen_id_) {
    screens_[active_screen_id_]->Close();
    active_screen_id_ = screen_id;
    screens_[active_screen_id_]->Show();
  }
}

std::shared_ptr<Screen> ScreenStateMachine::active_screen() {
  return screens_[active_screen_id_];
}
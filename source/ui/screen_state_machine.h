#ifndef AAIRACE_SOURCE_UI_SCREEN_STATE_MACHINE_H_
#define AAIRACE_SOURCE_UI_SCREEN_STATE_MACHINE_H_

#include <functional>
#include <memory>

#include <game/user_profile.h>

namespace ui {
class Screen;
struct GameWindowContext;
class ScreenStateMachine {
 public:
  ScreenStateMachine() = delete;
  ScreenStateMachine(const ScreenStateMachine& screen_state_machine) = delete;
  ScreenStateMachine(ScreenStateMachine&& screen_state_machine) = delete;
  ScreenStateMachine& operator=(const ScreenStateMachine& screen_state_machine) = delete;
  ScreenStateMachine& operator=(ScreenStateMachine&& screen_state_machine) = delete;

  ScreenStateMachine(const GameWindowContext& game_window_context,
                     const std::function<void()>& exit_game_function);
  ~ScreenStateMachine();

  void SetScreen(const std::shared_ptr<Screen>& screen);
  std::shared_ptr<Screen> active_screen();
  game::UserProfile& GetUserProfile();
  void ExitGame();
 private:
  std::function<void()> exit_game_function_;
  game::UserProfile active_user_profile_;
  std::shared_ptr<Screen> active_screen_;
};
}  // namespace ui

#endif  // AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_
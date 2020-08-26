#ifndef AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_
#define AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_

#include <memory>
#include <map>

class Screen;
struct GameWindowContext;
class ScreenStateMachine {
 public:
  ScreenStateMachine(const GameWindowContext& game_window_context);

  void SetScreen(const std::shared_ptr<Screen>& screen);
  std::shared_ptr<Screen> active_screen();
 private:
  std::shared_ptr<Screen> active_screen_;
};

#endif  // AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_
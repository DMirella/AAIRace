#ifndef AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_
#define AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_

#include <memory>
#include <map>

enum class ScreenId {
  kGameScreen = 0
};

class Screen;
struct GameWindowContext;
class ScreenStateMachine {
 public:
  ScreenStateMachine(const GameWindowContext& game_window_context);

  void SetScreen(ScreenId screen_id);

  std::shared_ptr<Screen> active_screen();
 private:
  ScreenId active_screen_id_;

  std::map<ScreenId, std::shared_ptr<Screen>> screens_;
};

#endif  // AAIRACE_SOURCE_SCREEN_STATE_MACHINE_H_
#ifndef AAIRACE_SOURCE_SCREEN_H_
#define AAIRACE_SOURCE_SCREEN_H_

#include <memory>

#include "level_manager.h"

class ScreenStateMachine;
class Screen {
 public:
  Screen(ScreenStateMachine* const screen_state_machine);

  virtual void Close() = 0;
  virtual void Show() = 0;

  virtual void NotifyGameCycleElapsed(float elapsed_time) { }
  virtual void Draw() { }
 protected:
  ScreenStateMachine* const screen_state_machine_;
};

class GameSession;
class GameScreen : public Screen {
 public:
  GameScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context);

  virtual void Close() override;
  virtual void Show() override;
  virtual void NotifyGameCycleElapsed(float elapsed_time) override;
  virtual void Draw() override;
 private:
  
  enum class InternalStateId {
    kLevelChoose = 0,
    kGameSessionActive
  };

  InternalStateId active_internal_state_id_;
  std::unique_ptr<GameSession> active_game_session_;
  LevelManager level_manager_;
};

#endif  // AAIRACE_SOURCE_SCREEN_H_
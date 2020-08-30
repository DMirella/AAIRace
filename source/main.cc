#include <cstdlib>
#include <ctime>

#include "ui/game_window.h"

int main() {
  std::srand(std::time(nullptr));

  ui::GameWindow::LoadGameTextures();
  
  ui::GameWindow game_window;
  game_window.Start();

  return 0;
}
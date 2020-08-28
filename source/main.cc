#include <cstdlib>
#include <ctime>
#include <iostream>

#include "ui/game_window.h"

int main() {
  std::srand(std::time(nullptr));

  GameWindow game_window;
  game_window.Start();

  return 0;
}
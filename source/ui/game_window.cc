#include "game_window.h"

#include <functional>

#include "game/level_manager.h"
#include "screens.h"

GameWindow::GameWindow() {
  window_.create(sf::VideoMode(1280, 720), "Lesson 4. kychka-pc.ru"); 

  auto draw_callback = std::bind(&GameWindow::Draw, this, std::placeholders::_1);
  GameWindowContext window_context;
  window_context.screen_width = static_cast<int>(window_.getSize().x);
  window_context.screen_height = static_cast<int>(window_.getSize().y);
  window_context.draw_function = draw_callback;

  screen_state_machine_ = std::make_unique<ScreenStateMachine>(window_context);
}

void GameWindow::Start() {
  sf::Clock clock;
  while (window_.isOpen()) {
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time / 200;
    sf::Event event;
    while (window_.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window_.close();
    }
    screen_state_machine_->active_screen()->NotifyGameCycleElapsed(time);

    window_.clear();
    screen_state_machine_->active_screen()->Draw();
    window_.display();
  }
}

void GameWindow::Draw(const sf::Drawable& object) {
  window_.draw(object);
}
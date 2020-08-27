#include "game_window.h"

#include <functional>
#include <iostream>

#include "game/level_manager.h"
#include "screens.h"

GameWindow::GameWindow() {
  // window_.create(sf::VideoMode(1280, 720), "Lesson 4. kychka-pc.ru"); 
  window_.create(sf::VideoMode(1280, 720), "Lesson 4. kychka-pc.ru", sf::Style::Fullscreen); 
  
  auto draw_callback = std::bind(&GameWindow::Draw, this, std::placeholders::_1);
  GameWindowContext window_context;
  window_context.screen_width = static_cast<int>(window_.getSize().x);
  window_context.screen_height = static_cast<int>(window_.getSize().y);
  window_context.draw_function = draw_callback;

  screen_state_machine_ = std::make_unique<ScreenStateMachine>(window_context);
}

void GameWindow::Start() {
  sf::Clock clock;
  UserControllersContext user_controllers_context;
  while (window_.isOpen()) {
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time = time / 200;
    sf::Event event;
    user_controllers_context = UserControllersContext();
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      } else if (event.type == sf::Event::TextEntered) {
        user_controllers_context.entered_unicode = event.text.unicode;
      }
    }
    user_controllers_context.is_mouse_button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    user_controllers_context.cursor_x = sf::Mouse::getPosition(window_).x;
    user_controllers_context.cursor_y = sf::Mouse::getPosition(window_).y;
    screen_state_machine_->active_screen()->NotifyGameCycleElapsed(time, user_controllers_context);

    window_.clear(sf::Color(0x80, 0xA4, 0x86));
    screen_state_machine_->active_screen()->Draw();
    window_.display();
  }
}

void GameWindow::Draw(const sf::Drawable& object) {
  window_.draw(object);
}
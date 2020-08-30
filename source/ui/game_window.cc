#include "game_window.h"

#include <functional>
#include <iostream>
#include <string>

#include <game/units/city_car.h>
#include <game/units/racing_car.h>
#include <game/level_manager.h>

#include "screen_state_machine.h"
#include "screens.h"

namespace ui {
GameWindow::GameWindow() {
  const sf::VideoMode kGameVideoMode = sf::VideoMode(1280, 720);
  const std::string kGameWindowTitle = "AAIRace";
  const bool kIsFullScreen = false;

  window_.create(kGameVideoMode, kGameWindowTitle, kIsFullScreen? sf::Style::Fullscreen : sf::Style::Default);
  
  auto draw_callback = std::bind(&GameWindow::Draw, this, std::placeholders::_1);
  auto exit_game_callback = std::bind(&GameWindow::ExitGame, this);
  GameWindowContext window_context;
  window_context.screen_width = static_cast<int>(window_.getSize().x);
  window_context.screen_height = static_cast<int>(window_.getSize().y);
  window_context.draw_function = draw_callback;

  screen_state_machine_ = std::make_shared<ScreenStateMachine>(window_context, exit_game_callback);
}

void GameWindow::LoadGameTextures() {
  game::units::RacingCar::Init();
  game::units::CityCar::Init();
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
        ExitGame();
      } else if (event.type == sf::Event::TextEntered) {
        user_controllers_context.entered_unicode = event.text.unicode;
      }
    }
    user_controllers_context.is_left_arrow_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    user_controllers_context.is_right_arrow_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    user_controllers_context.is_up_arrow_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    user_controllers_context.is_down_arrow_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    user_controllers_context.is_mouse_button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    user_controllers_context.cursor_x = sf::Mouse::getPosition(window_).x;
    user_controllers_context.cursor_y = sf::Mouse::getPosition(window_).y;
    screen_state_machine_->active_screen()->NotifyGameCycleElapsed(time, user_controllers_context);

    window_.clear(sf::Color(0x80, 0xA4, 0x86));
    screen_state_machine_->active_screen()->Draw();
    window_.display();
  }
}

void GameWindow::ExitGame() {
  window_.close();
}

void GameWindow::Draw(const sf::Drawable& object) {
  window_.draw(object);
}
}  // namespace ui
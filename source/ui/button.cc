#include "button.h"

#include <iostream>

sf::Color Button::kBasicRectColor = sf::Color(0x28, 0x37, 0x2D);
sf::Color Button::kActiveRectColor = sf::Color(0x33, 0x49, 0x37);
sf::Color Button::kBasicFontColor = sf::Color::White;
sf::Color Button::kActiveFontColor = sf::Color::White;

Button::Button(int x, int y, int width, int height, const std::string& text_string,
               const OnClickCallback& on_press_callback, const DrawFunction& draw_function)
    : DrawableUnit(draw_function)
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
    , on_press_callback_(on_press_callback)
    , is_mouse_down_on_active_button_(false) {
  const std::string kPathToFont = "resources/button_font.ttf";
  const int kFontSize = height / 1.5f;

  font_.loadFromFile(kPathToFont);
  text_.setString(text_string);
  text_.setFont(font_);
  text_.setColor(sf::Color::White);
  text_.setCharacterSize(kFontSize);
  // text_.setPosition(0, 0);
  auto local_text_bounds = text_.getLocalBounds();
  text_.setPosition(x_ + (width - local_text_bounds.width) / 2, y_ + (height - local_text_bounds.height) / 2 - kFontSize * 0.3);
  rect_.setSize(sf::Vector2f(width_, height_));
  rect_.setFillColor(sf::Color(100, 100, 100));
  rect_.setPosition(x_, y_);
}

void Button::Draw() {
  draw_function_(rect_);
  draw_function_(text_);
}

void Button::Update(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) {
  // std::cout << cursor_x << " " << cursor_y << std::endl;
  if (x_ <= cursor_x && cursor_x <= x_ + width_
      && y_ <= cursor_y && cursor_y <= y_ + height_) {
    if (is_mouse_button_pressed && !is_mouse_down_on_active_button_) {
      is_mouse_down_on_active_button_ = true;
    } else if (!is_mouse_button_pressed && is_mouse_down_on_active_button_) {
      is_mouse_down_on_active_button_ = false;
      on_press_callback_();
    }
    rect_.setFillColor(kActiveRectColor);
    text_.setColor(kActiveFontColor);
  } else {
    rect_.setFillColor(kBasicRectColor);
    text_.setColor(kBasicFontColor);
  }
}
#include "button.h"

#include <iostream>

#include <common/tools.h>
#include "center_align_label.h"

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
  const int kFontSize = height / 1.5f;

  text_label_ = std::make_shared<CenterAlignLabel>(tools::Rectangle(x_, y_, x_ + width_, y_ + height_),
                                                   text_string, kFontSize, draw_function_);
  rect_.setSize(sf::Vector2f(width_, height_));
  rect_.setFillColor(kBasicRectColor);
  rect_.setPosition(x_, y_);
}

void Button::Draw() {
  draw_function_(rect_);
  text_label_->Draw();
}

void Button::Update(float elapsed_time, const UserControllersContext& context) {
  if (x_ <= context.cursor_x && context.cursor_x <= x_ + width_ &&
      y_ <= context.cursor_y && context.cursor_y <= y_ + height_) {
    if (context.is_mouse_button_pressed && !is_mouse_down_on_active_button_) {
      is_mouse_down_on_active_button_ = true;
    } else if (!context.is_mouse_button_pressed && is_mouse_down_on_active_button_) {
      is_mouse_down_on_active_button_ = false;
      on_press_callback_();
    }
    rect_.setFillColor(kActiveRectColor);
  } else {
    rect_.setFillColor(kBasicRectColor);
  }
}
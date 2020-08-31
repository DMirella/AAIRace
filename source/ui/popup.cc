#include "popup.h"

namespace {
const sf::Color gBasicRectColor = sf::Color(0, 0, 0, 225);
}  // namespace 

namespace ui {
Popup::Popup(int x, int y, int width, int height, 
             const Button::OnClickCallback& on_ok_button_click_callback, 
             const DrawableUnit::DrawFunction& draw_function)
    : DrawableUnit(draw_function) {
  const int kFontSize = 35;
  const int kButtonWidth = 80;
  const int kButtonHeight = 40;
  const int kButtonXYOffset = 10;
  const std::string kOKButtonText = "OK";

  ok_button_ = std::make_unique<Button>(
      x + (width - kButtonWidth) / 2, y + height - kButtonHeight
          - kButtonXYOffset, kButtonWidth, kButtonHeight, 
      kOKButtonText, on_ok_button_click_callback, draw_function);
  rectangle_.setSize(sf::Vector2f(width, height));
  rectangle_.setFillColor(gBasicRectColor);
  rectangle_.setPosition(x, y);
}

void Popup::Draw() {
  draw_function_(rectangle_);
  ok_button_->Draw();
}

void Popup::Update(float elapsed_time, const UserControllersContext& context) {
  ok_button_->Update(elapsed_time, context);
}
}  // namespace ui
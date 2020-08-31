#include "text_popup.h"

namespace ui {
TextPopup::TextPopup(int x, int y, int width, int height, 
                     const Button::OnClickCallback& on_ok_button_click_callback, 
                     const DrawableUnit::DrawFunction& draw_function,
                     const std::string& message)
    : Popup(x, y, width, height, on_ok_button_click_callback, draw_function) {
  const int kFontSize = 35;
  message_label_ = std::make_unique<CenterAlignLabel>(
      common::Rectangle(x, y, x + width, y + height - kButtonHeight),
      message, kFontSize, draw_function);
}

void TextPopup::Draw() {
  Popup::Draw();
  message_label_->Draw();
}

void TextPopup::Update(float elapsed_time, const UserControllersContext& context) {
  Popup::Update(elapsed_time, context);
  message_label_->Update(elapsed_time, context);
}
}  // namespace ui
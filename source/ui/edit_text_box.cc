#include "edit_text_box.h"

#include <iostream>

#include "center_align_label.h"

namespace {
sf::Color gBackgroundRectColor = sf::Color(0x5A, 0x7C, 0x60);
const int gTextXOffset = 10;
}  // namespace

namespace ui {
EditTextBox::EditTextBox(int x, int y, int width, int height, const DrawFunction& draw_function)
    : DrawableUnit(draw_function)
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
    , entered_string_("") {
  const int kFontHeight = height_ / 1.5f;
  
  background_rect_.setSize(sf::Vector2f(width_, height_));
  background_rect_.setFillColor(gBackgroundRectColor);
  background_rect_.setPosition(x_, y_);

  text_label_ = std::make_unique<CenterAlignLabel>(
      common::Rectangle(x_ + gTextXOffset, y_, x_ + width_ - gTextXOffset, y_ + height_),
      entered_string_, kFontHeight, draw_function_);
}

void EditTextBox::Draw() {
  draw_function_(background_rect_);
  text_label_->Draw();
}

void EditTextBox::Update(float elapsed_time, const UserControllersContext& context) {
  if (CheckValidInput(context.entered_unicode)) {
    if(context.entered_unicode == '\b') {
      entered_string_ = entered_string_.substr(0, entered_string_.length() - 1);
    } else {
      entered_string_ += static_cast<char>(context.entered_unicode);
    }
    text_label_->SetText(entered_string_);
    if (text_label_->text_pixel_width() >= width_ - 2 * gTextXOffset) {
      entered_string_ = entered_string_.substr(0, entered_string_.length() - 1);
      text_label_->SetText(entered_string_);
    }
  }
}

std::string EditTextBox::entered_string() const {
  return entered_string_;
}

bool EditTextBox::CheckValidInput(sf::Uint32 unicode) const {
  return ('a' <= unicode && unicode <= 'z') ||
         ('A' <= unicode && unicode <= 'Z') ||
         ('0' <= unicode && unicode <= '9') ||
         unicode == '\b';
}
}  // namespace ui
#include "center_align_label.h"

CenterAlignLabel::CenterAlignLabel(const tools::Rectangle& rect, const std::string& text_string, 
                                   int font_size, const DrawFunction& draw_function)
    : DrawableUnit(draw_function)
    , kFontSize(font_size)
    , bound_rectangle_(rect) {
  const std::string kPathToFont = "resources/button_font.ttf";
  const sf::Color kTextColor = sf::Color::White;

  font_.loadFromFile(kPathToFont);
  text_.setString(text_string);
  text_.setFont(font_);
  text_.setColor(kTextColor);
  text_.setCharacterSize(kFontSize);
  auto local_text_bounds = text_.getLocalBounds();
  text_y_ = rect.y1 + (rect.y2 - rect.y1 - local_text_bounds.height) / 2 - local_text_bounds.height * 0.5;
  text_.setPosition(rect.x1 + (rect.x2 - rect.x1 - local_text_bounds.width) / 2, 
                    text_y_);
}

void CenterAlignLabel::SetText(const std::string& text_string) {
  text_.setString(text_string);
  auto local_text_bounds = text_.getLocalBounds();
  text_.setPosition(bound_rectangle_.x1 + (bound_rectangle_.x2 - bound_rectangle_.x1 - local_text_bounds.width) / 2, 
                    text_y_);
}

void CenterAlignLabel::SetTextColor(const sf::Color& color) {
  text_.setColor(color);
}

void CenterAlignLabel::Draw() {
  draw_function_(text_);
}

int CenterAlignLabel::text_pixel_width() const {
  auto local_text_bounds = text_.getLocalBounds();
  return local_text_bounds.width;
}
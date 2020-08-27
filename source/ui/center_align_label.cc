#include "center_align_label.h"

CenterAlignLabel::CenterAlignLabel(const tools::Rectangle& rect, const std::string& text_string, 
                                   int font_size, const DrawFunction& draw_function)
    : DrawableUnit(draw_function) {
  const std::string kPathToFont = "resources/button_font.ttf";
  const sf::Color kTextColor = sf::Color::White;

  font_.loadFromFile(kPathToFont);
  text_.setString(text_string);
  text_.setFont(font_);
  text_.setColor(kTextColor);
  text_.setCharacterSize(font_size);
  auto local_text_bounds = text_.getLocalBounds();
  text_.setPosition(rect.x1 + (rect.x2 - rect.x1 - local_text_bounds.width) / 2, 
                    rect.y1 + (rect.y2 - rect.y1 - local_text_bounds.height) / 2 - font_size * 0.3);
}

void CenterAlignLabel::Draw() {
  draw_function_(text_);
}
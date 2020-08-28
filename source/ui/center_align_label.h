#ifndef SOURCE_UI_CENTER_ALIGN_LABEL_H_
#define SOURCE_UI_CENTER_ALIGN_LABEL_H_

#include <string>

#include <SFML/Graphics.hpp>

#include <common/tools.h>
#include <common/drawable_unit.h>

class CenterAlignLabel : public DrawableUnit {
 public:
  CenterAlignLabel(const tools::Rectangle& rect, const std::string& text_string, 
                   int font_size, const DrawFunction& draw_function);

  void SetText(const std::string& text_string);
  void SetTextColor(const sf::Color& color);

  // DrawableUnit
  virtual void Draw() override;

  int text_pixel_width() const;
 private:
  const int kFontSize;

  sf::Font font_;
  sf::Text text_;
  int text_y_;
  tools::Rectangle bound_rectangle_;
};

#endif  // SOURCE_UI_CENTER_ALIGN_LABEL_H_
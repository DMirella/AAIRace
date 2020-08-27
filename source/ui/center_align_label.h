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

  virtual void Draw() override;
 private:
  sf::Font font_;
  sf::Text text_;
};

#endif  // SOURCE_UI_CENTER_ALIGN_LABEL_H_
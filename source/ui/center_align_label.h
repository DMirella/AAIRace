#ifndef SOURCE_UI_CENTER_ALIGN_LABEL_H_
#define SOURCE_UI_CENTER_ALIGN_LABEL_H_

#include <string>

#include <SFML/Graphics.hpp>

#include <common/tools.h>
#include <common/drawable_unit.h>

namespace ui {
class CenterAlignLabel : public common::DrawableUnit {
 public:
  CenterAlignLabel() = delete;
  CenterAlignLabel(const CenterAlignLabel& center_align_label) = delete;
  CenterAlignLabel(CenterAlignLabel&& center_align_label) = delete;
  CenterAlignLabel& operator=(const CenterAlignLabel& center_align_label) = delete;
  CenterAlignLabel& operator=(CenterAlignLabel&& center_align_label) = delete;

  CenterAlignLabel(const common::Rectangle& rect, const std::string& text_string, 
                   int font_size, const DrawFunction& draw_function);

  void SetText(const std::string& text_string);
  void SetTextColor(const sf::Color& color);
  void SetOutlineBorder(int size);

  // DrawableUnit
  virtual void Draw() override;

  int text_pixel_width() const;
 private:
  const int kFontSize;
  sf::Font font_;
  sf::Text text_;
  int text_y_;
  common::Rectangle bound_rectangle_;
};
}  // namespace ui

#endif  // SOURCE_UI_CENTER_ALIGN_LABEL_H_
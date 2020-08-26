#ifndef AAIRACE_SOURCE_UI_BUTTON_H_
#define AAIRACE_SOURCE_UI_BUTTON_H_

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

class Button : public DrawableUnit {
 public:
  using OnClickCallback = std::function<void()>;

  Button(int x, int y, int width, int height, const std::string& text_string,
         const OnClickCallback& on_press_callback, const DrawableUnit::DrawFunction& draw_function);
  
  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time, int cursor_x, int cursor_y, bool is_mouse_button_pressed) override;
 private:
  static sf::Color kBasicRectColor;
  static sf::Color kActiveRectColor;
  static sf::Color kBasicFontColor;
  static sf::Color kActiveFontColor;
       
  int x_;
  int y_;
  int width_;
  int height_;
  sf::Font font_;
  sf::Text text_;
  sf::RectangleShape rect_;

  bool is_mouse_down_on_active_button_;

  OnClickCallback on_press_callback_;
};

#endif  // AAIRACE_SOURCE_UI_BUTTON_H_
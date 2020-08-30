#ifndef AAIRACE_SOURCE_UI_BUTTON_H_
#define AAIRACE_SOURCE_UI_BUTTON_H_

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>
#include "center_align_label.h"

class Button : public DrawableUnit {
 public:
  using OnClickCallback = std::function<void()>;
  
  Button() = delete;
  Button(const Button& button) = delete;
  Button(Button&& button) = delete;
  Button& operator=(const Button& button) = delete;
  Button& operator=(Button&& button) = delete;

  Button(int x, int y, int width, int height, const std::string& text_string,
         const OnClickCallback& on_press_callback, const DrawableUnit::DrawFunction& draw_function);

  void SetEnable(bool enable);
  
  // DrawableUnit
  virtual void Draw() override;
  virtual void Update(float elapsed_time, const UserControllersContext& context) override;
 private:
  int x_;
  int y_;
  int width_;
  int height_;
  std::unique_ptr<CenterAlignLabel> text_label_;
  sf::RectangleShape rect_;

  bool enable_;
  bool is_mouse_down_on_active_button_;

  OnClickCallback on_press_callback_;
};

#endif  // AAIRACE_SOURCE_UI_BUTTON_H_
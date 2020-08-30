#ifndef AAIRACE_SOURCE_COMMON_DRAWABLE_UNIT_H_
#define AAIRACE_SOURCE_COMMON_DRAWABLE_UNIT_H_

#include <functional>

namespace sf {
class Drawable;
}  // namespace sf

namespace ui {
struct UserControllersContext {
  int cursor_x;
  int cursor_y;
  bool is_left_arrow_pressed;
  bool is_right_arrow_pressed;
  bool is_up_arrow_pressed;
  bool is_down_arrow_pressed;
  bool is_mouse_button_pressed;
  sf::Uint32 entered_unicode;
};
}  // namespace ui

namespace common {

class DrawableUnit {
 public:
  using DrawFunction = std::function<void(const sf::Drawable& drawable)>;
  
  DrawableUnit(const DrawFunction& draw_function) : draw_function_(draw_function) {}
  virtual ~DrawableUnit() {}

  virtual void Draw() = 0;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) {}
 protected:
  DrawFunction draw_function_;
};
}  // namespace common

#endif  // AAIRACE_SOURCE_COMMON_DRAWABLE_UNIT_H_
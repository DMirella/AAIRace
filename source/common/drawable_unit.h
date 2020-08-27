#ifndef AAIRACE_SOURCE_DRAWABLE_UNIT_H_
#define AAIRACE_SOURCE_DRAWABLE_UNIT_H_

#include <memory>
#include <functional>

namespace sf {
class Sprite;
}  // namespace sf

struct UserControllersContext {
  int cursor_x;
  int cursor_y;
  bool is_mouse_button_pressed;
  sf::Uint32 entered_unicode;
};

class DrawableUnit {
 public:
  using DrawFunction = std::function<void(const sf::Drawable& sprite)>;
  
  DrawableUnit(const DrawFunction& draw_function) : draw_function_(draw_function) {}
  virtual ~DrawableUnit() {}

  virtual void Draw() = 0;
  virtual void Update(float elapsed_time, const UserControllersContext& context) {}
 protected:
  DrawFunction draw_function_;
};

#endif  // AAIRACE_SOURCE_DRAWABLE_UNIT_H_
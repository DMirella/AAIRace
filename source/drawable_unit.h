#ifndef AAIRACE_SOURCE_DRAWABLE_UNIT_H_
#define AAIRACE_SOURCE_DRAWABLE_UNIT_H_

#include <memory>

namespace sf {
class RenderWindow;
}  // namespace sf

class DrawableUnit {
 public:
  DrawableUnit(const std::shared_ptr<sf::RenderWindow>& render_window) : render_window_(render_window) {}
  virtual ~DrawableUnit() {}

  virtual void Draw() = 0;
  virtual void Update(float elapsed_time) = 0;
 protected:
  std::shared_ptr<sf::RenderWindow> render_window_;
};

#endif  // AAIRACE_SOURCE_DRAWABLE_UNIT_H_
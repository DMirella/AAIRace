#ifndef AAIRACE_SOURCE_UI_POPUP_H_
#define AAIRACE_SOURCE_UI_POPUP_H_

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

#include "button.h"
#include "center_align_label.h"

namespace ui {
class Popup : public common::DrawableUnit {
 public:
  Popup() = delete;
  Popup(const Popup& popup) = delete;
  Popup(Popup&& popup) = delete;
  Popup& operator=(const Popup& popup) = delete;
  Popup& operator=(Popup&& popup) = delete;

  Popup(int x, int y, int width, int height,
        const Button::OnClickCallback& on_ok_button_click_callback,
        const DrawFunction& draw_function);

  virtual void Draw() override;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;
 protected:
  const int kButtonWidth = 80;
  const int kButtonHeight = 40;

  std::unique_ptr<Button> ok_button_;
  sf::RectangleShape rectangle_;
};
}  // namespace ui

#endif  // AAIRACE_SOURCE_UI_POPUP_H_
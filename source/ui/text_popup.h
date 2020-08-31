#ifndef AAIRACE_SOURCE_UI_TEXT_POPUP_H_
#define AAIRACE_SOURCE_UI_TEXT_POPUP_H_

#include <SFML/Graphics.hpp>

#include <memory>

#include "button.h"
#include "center_align_label.h"
#include "popup.h"

namespace ui {
class TextPopup : public Popup {
 public:
  TextPopup() = delete;
  TextPopup(const TextPopup& popup) = delete;
  TextPopup(TextPopup&& popup) = delete;
  TextPopup& operator=(const TextPopup& popup) = delete;
  TextPopup& operator=(TextPopup&& popup) = delete;

  TextPopup(int x, int y, int width, int height,
            const Button::OnClickCallback& on_ok_button_click_callback,
            const DrawFunction& draw_function, const std::string& message);

  virtual void Draw() override;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;
 private:
  std::unique_ptr<CenterAlignLabel> message_label_;
};
}  // namespace ui

#endif  // AAIRACE_SOURCE_UI_TEXT_POPUP_H_
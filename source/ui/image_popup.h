#ifndef AAIRACE_SOURCE_UI_IMAGE_POPUP_H_
#define AAIRACE_SOURCE_UI_IMAGE_POPUP_H_

#include <SFML/Graphics.hpp>

#include <memory>

#include "button.h"
#include "center_align_label.h"
#include "popup.h"

namespace ui {
class ImagePopup : public Popup {
 public:
  ImagePopup() = delete;
  ImagePopup(const ImagePopup& popup) = delete;
  ImagePopup(ImagePopup&& popup) = delete;
  ImagePopup& operator=(const ImagePopup& popup) = delete;
  ImagePopup& operator=(ImagePopup&& popup) = delete;

  ImagePopup(int x, int y, int width, int height,
            const Button::OnClickCallback& on_ok_button_click_callback,
            const DrawFunction& draw_function, const sf::Image& image);

  virtual void Draw() override;
  virtual void Update(float elapsed_time, const ui::UserControllersContext& context) override;
 private:
  sf::Texture texture_;
  sf::Sprite sprite_;
};
}  // namespace ui

#endif  // AAIRACE_SOURCE_UI_IMAGE_POPUP_H_
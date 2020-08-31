#include "image_popup.h"

namespace ui {
ImagePopup::ImagePopup(int x, int y, int width, int height, 
                       const Button::OnClickCallback& on_ok_button_click_callback, 
                       const DrawableUnit::DrawFunction& draw_function,
                       const sf::Image& image)
    : Popup(x, y, width, height, on_ok_button_click_callback, draw_function) {
  const int kImageOffsetY = (height - image.getSize().y) / 2;
  const int kImageOffsetX = (width - image.getSize().x) / 2;
  
  texture_.loadFromImage(image);
  sprite_.setTexture(texture_);
  sprite_.setPosition(sf::Vector2f(x + kImageOffsetX, y + kImageOffsetY));
}

void ImagePopup::Draw() {
  Popup::Draw();
  draw_function_(sprite_);
}

void ImagePopup::Update(float elapsed_time, const UserControllersContext& context) {
  Popup::Update(elapsed_time, context);
}
}  // namespace ui
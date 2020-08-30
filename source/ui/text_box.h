#ifndef SOURCE_UI_TEXT_BOX_H_
#define SOURCE_UI_TEXT_BOX_H_

#include <string>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>

class CenterAlignLabel;
class TextBox : public DrawableUnit {
public:
  TextBox() = delete;
  TextBox(const TextBox& text_box) = delete;
  TextBox(TextBox&& text_box) = delete;
  TextBox& operator=(const TextBox& text_box) = delete;
  TextBox& operator=(TextBox&& text_box) = delete;

  TextBox(int x, int y, int width, int height, const DrawFunction& draw_function);
	
  virtual void Draw() override;
  virtual void Update(float elapsed_time, const UserControllersContext& context) override;

	std::string entered_string() const;
private:
  bool CheckValidInput(sf::Uint32 unicode) const;

	int x_;
	int y_;
	int width_;
	int height_;
	std::string entered_string_;
	sf::Font font;
	sf::RectangleShape background_rect_;
	std::shared_ptr<CenterAlignLabel> text_label_;
};

#endif  // SOURCE_UI_TEXT_BOX_H_
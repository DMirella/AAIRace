#ifndef SOURCE_UI_EDIT_TEXT_BOX_H_
#define SOURCE_UI_EDIT_TEXT_BOX_H_

#include <string>

#include <SFML/Graphics.hpp>

#include <common/drawable_unit.h>
#include "center_align_label.h"

class EditTextBox : public DrawableUnit {
public:
  EditTextBox() = delete;
  EditTextBox(const EditTextBox& text_box) = delete;
  EditTextBox(EditTextBox&& text_box) = delete;
  EditTextBox& operator=(const EditTextBox& text_box) = delete;
  EditTextBox& operator=(EditTextBox&& text_box) = delete;

  EditTextBox(int x, int y, int width, int height, const DrawFunction& draw_function);
	
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
	std::unique_ptr<CenterAlignLabel> text_label_;
};

#endif  // SOURCE_UI_EDIT_TEXT_BOX_H_
//
// Created by quan on 27/10/2019.
//

#include "button.h"

bool button::update(sf::Vector2f mouse_world_pos) {
	// nếu chuột rê vào nút ấn thì đổi màu nó
	if (bounds.contains(mouse_world_pos)) {
		text.setFillColor(hover_color);
		return true;
	}
	text.setFillColor(sf::Color::White);
	return false;
}

button::button(sf::Text& text, sf::Color hover_color) : text(text), hover_color(hover_color) {
	bounds = text.getGlobalBounds();
}
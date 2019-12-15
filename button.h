//
// Created by quan on 27/10/2019.
//

#ifndef OOP_PROJECT01_BUTTON_H
#define OOP_PROJECT01_BUTTON_H


#include <SFML/Graphics.hpp>

// class để quản lý các nút ấn trong game
class button {
	// chữ hiển thị của nút
	sf::Text& text;
	// biên của nút (để kiểm tra chuột có vào biên không
	sf::FloatRect bounds;
	// màu của nút khi rê chuột vào
	sf::Color hover_color;

public:
	// update nút ấn và trả về true nếu chuột nằm trong nút ấn
	// mouse_world_pos: vị trí của theo tọa độ của game
	bool update(sf::Vector2f mouse_world_pos);
	// khởi tạo nút ấn
	// text: chữ hiển thị của nút ấn
	// hover_color: màu của nút khi rê chuột vào
	button(sf::Text& text, sf::Color hover_color);
};


#endif //OOP_PROJECT01_BUTTON_H
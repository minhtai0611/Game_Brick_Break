//
// Created by quan on 26/10/2019.
//

#ifndef CTDL_PROJECT01__GAME_DATA_H
#define CTDL_PROJECT01__GAME_DATA_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include"ball.h"
#include<algorithm>
using namespace sf;
// kiểm tra nếu value < min thì trả về -1, nếu value > max thì trả về 1, còn lại trả về 0
int out_of_bounds(float value, float min, float max);

// đám bảo giá trị trả về luôn nàm trong khoảng [min, max]
float coerce_in(float value, float min, float max);

// đám bảo trị tuyệt đối của giá trị trả về bé hơn max_abs
float coerce_in(float value, float max_abs);

// đặt vị trí cho item dựa theo giá trị x_align và y_align:
// nếu x_align_type == -1 thì canh phải item tại vị trí x_align
// nếu bằng 0 thì canh giữa, nếu bằng 1 thì canh trái
// tương tự với y_align_type
void align(sf::Text& item, float x_align, int x_align_type, float y_align, int y_align_type);

// đặt vị trí cho item dựa vào giá trị x và y của align_to
void align(sf::Text& item, sf::Vector2f align_to, int x_align_type, int y_align_type);

extern class _game_data {
	void init_menu_item(sf::Text& item, const std::string& name);
public:
	// các kích thước của người chơi
	float p_size, p_thickness, p_gem_gap, p_gem_extrude;
	// khoảng các hiển thị điểm người chơi so với sân chơi
	float p_score_gap;
	// tốc đổ tối đa, tối thiểu và gia tốc của người chơi
	float p_min_speed, p_max_speed, p_accel;
	// số điểm ban đầu
	int p_score;

	// lề trên và bên trái của sân chơi (cũng là góc trên bên trái)
	sf::Vector2f play_pos_min;
	// lề dưới bên phải của sân chơi
	sf::Vector2f play_margin_max;
	// vị trí góc dưới bên phải của sân chơi
	sf::Vector2f play_pos_max;
	// vị trí tâm sân chơi
	sf::Vector2f play_pos_center;
	// các phông chứ đậm, chữ nhạt, chữ viết tay
	sf::Font thick_font, thin_font, cursive_font;
	// thời gian đợi của các sự kiện
	float delay_time;
	// có chơi game toàn màn hình
	bool fullscreen;
	float font_multiplier;

	// điều khiển phải trái cho người chơi 1 và 2
	sf::Keyboard::Key p1_negative, p1_positive, p2_negative, p2_positive;
	// màu của người chơi 1 và 2
	sf::Color p1_color, p2_color;
	// vị trí của người chơi 1 và 2
	float p1_y, p2_y;

	// các kích thước của banh
	float b_size, b_decor;
	// tốc độ di chuyển, tốc độ quay và giảm quay của banh
	float b_speed, b_spin, b_spin_decrease;

	// khởi tạo game_data
	explicit _game_data();

	// khởi tạo các thông số phụ thuộc kích thước cửa sổ
	void set_window(sf::RenderWindow& window);

	// tạo các shape cho người chơi
	// top_player: tạo thông tin cho người chơi trên hay dưới 
	std::vector<sf::Shape*>* player_shape(bool top_player);

	// tạo các shape cho banh
	std::vector<sf::Shape*>* ball_shape();

	// tạo các shape để hiện điểm người chơi
	// top_player: tạo shape cho người chơi trên hay dưới 
	sf::Text* player_score(bool top_player);

	// tạo menu game over
	std::vector<sf::Text>* game_over_text();

	// tạo menu chính
	std::vector<sf::Text>* main_menu_text();

	// tạo menu tam dừng
	std::vector<sf::Text>* pause_menu_text();

	// kiểm tra một đối tượng ở vị trí pos_x, kích thước size_x có ra ngoài biên dọc không 
	bool out_of_bounds_x(float pos_x, float size_x);
	// kiểm tra một đối tượng ở vị trí pos_y, kích thước size_y có ra ngoài biên dọc không 
	int out_of_bounds_y(float pos_x, float size_x);
	// giới hạn tọa độ x của một vật kích thước size_x sao cho nó nằm trong biên dọc
	// pos_x: tọa độ x của vật đó 
	// trả về: tọa độ x được đảm bảo nằm trong biên 
	float coerce_in_x(float pos_x, float size_x);
	// giới hạn tọa độ y của một vật kích thước size_y sao cho nó nằm trong biên dọc
	// pos_y: tọa độ y của vật đó 
	// trả về: tọa độ y được đảm bảo nằm trong biên 
	float coerce_in_y(float pos_y, float size_y);
} *game_data;
void init_game_data();


#endif //CTDL_PROJECT01__GAME_DATA
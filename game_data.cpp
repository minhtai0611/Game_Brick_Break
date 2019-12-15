//
// Created by quan on 26/10/2019.
//

#include "game_data.h"

_game_data* game_data;

std::vector<sf::Shape*>* _game_data::player_shape(bool top_player) {
	// vẽ nên người chơi
	auto plate = new sf::ConvexShape(7);
	plate->setPoint(0, sf::Vector2f(-p_size, 0));
	plate->setPoint(1, sf::Vector2f(p_size, 0));
	plate->setPoint(2, sf::Vector2f(p_size - p_thickness, p_thickness + 2));
	plate->setPoint(3, sf::Vector2f(p_thickness, p_thickness));
	plate->setPoint(4, sf::Vector2f(0, p_thickness * p_gem_extrude));
	plate->setPoint(5, sf::Vector2f(-p_thickness, p_thickness));
	plate->setPoint(6, sf::Vector2f(-p_size + p_thickness, p_thickness + 2));
	plate->setFillColor(sf::Color(240, 240, 240));

	// vẽ đã trang trí
	auto gem = new sf::ConvexShape(5);
	gem->setPoint(0, sf::Vector2f((p_thickness - p_gem_gap) - 1, p_gem_gap / 2));
	gem->setPoint(1, sf::Vector2f((p_thickness - p_gem_gap + 1), p_thickness - 1));
	gem->setPoint(2, sf::Vector2f(0, p_thickness * p_gem_extrude - p_gem_gap + 1));
	gem->setPoint(3, sf::Vector2f(-(p_thickness - p_gem_gap + 1), p_thickness - 1));
	gem->setPoint(4, sf::Vector2f(-(p_thickness - p_gem_gap) + 1, p_gem_gap / 2));
	gem->setFillColor(top_player ? p1_color : p2_color);

	// trả vê vector chứa shape
	auto result = new std::vector<sf::Shape*>(0);
	result->push_back(plate);
	result->push_back(gem);
	return result;
}

std::vector<sf::Shape*>* _game_data::ball_shape() {
	// vẽ nền banh hình vuông
	auto ball = new sf::CircleShape(b_size,3);
	ball->setFillColor(sf::Color::Magenta);
	ball->setOrigin(b_size, b_size);
	//    auto decor = new sf::CircleShape(b_decor, 8);
	//    decor->setFillColor(sf::Color::Blue);
	//    decor->setOrigin(b_decor, b_decor);
		// vẽ trang trí banh
	auto result = new std::vector<sf::Shape*>(0);
	result->push_back(ball);
	//result->push_back(decor);
	return result;
}
// gán các setting cho game
_game_data::_game_data() {
	thick_font.loadFromFile("D:\Montserrat-Black.ttf");
	thin_font.loadFromFile("D:\OpenSans-Light.ttf");
	cursive_font.loadFromFile("D:\Satisfy-Regular.ttf");
	fullscreen = false;

	p_size = 50.f;
	p_thickness = 11.f;
	p_gem_gap = 4.5f;
	p_gem_extrude = 1.7;
	p_min_speed = 250;
	p_max_speed = 600;
	p_accel = 250;
	p_score = 5;
	p_score_gap = 40;

	p1_negative = sf::Keyboard::Left;
	p1_positive = sf::Keyboard::Right;
	p2_negative = sf::Keyboard::A;
	p2_positive = sf::Keyboard::D;
	p1_color = sf::Color::Cyan;
	p2_color = sf::Color(466, 250, 277);

	b_size = 15;
	b_decor = 5;
	b_spin = 420;
	b_spin_decrease = 120;
	delay_time = 1;
}

void _game_data::set_window(sf::RenderWindow& window) {
	auto size = window.getSize();
	play_pos_min = sf::Vector2f(0, 100);
	play_margin_max = sf::Vector2f(0, 100);
	play_pos_max = sf::Vector2f(size.x, size.y) - play_margin_max;
	play_pos_center = (play_pos_min + play_pos_max) / 2.f;
	p1_y = play_pos_min.y;
	p2_y = play_pos_max.y;
	// màn hình càng lớn thì banh đi càng nhanh, để tăng độ khó cho game
	b_speed = size.y / 3.f;
	font_multiplier = size.y / 800.f;
}

bool _game_data::out_of_bounds_x(float pos_x, float size_x) {
	return out_of_bounds(pos_x, play_pos_min.x + size_x, play_pos_max.x - size_x);
}

float coerce_in(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

float _game_data::coerce_in_x(float pos_x, float size_x) {
	return coerce_in(pos_x, play_pos_min.x + size_x, play_pos_max.x - size_x);
}

int out_of_bounds(float value, float min, float max) {
	if (value < min)
		return -1;
	if (value > max)
		return 1;
	return 0;
}

int _game_data::out_of_bounds_y(float pos_y, float size_y) {
	return out_of_bounds(pos_y, play_pos_min.y + size_y, play_pos_max.y - size_y);
}

float _game_data::coerce_in_y(float pos_y, float size_y) {
	return coerce_in(pos_y, play_pos_min.y + size_y, play_pos_max.y - size_y);
}

void init_game_data() {
	game_data = new _game_data();
}

float coerce_in(float value, float max_abs) {
	return coerce_in(value, -max_abs, max_abs);
}
// canh giá trị theo to_value, dựa vào vị trí và kích thước local của nó, và align_type:
// 0: canh giữa
// -1: canh nhỏ hơn (canh phải hoặc canh dưới)
// 1: canh lớn hơn (canh trái và canh trên)
// giá trị khác: giữ nguyên giá trị ban đầu
float align(float to_value, int align_type, float position, float size, float original) {
	switch (align_type) {
	case 0:
		return to_value - (position + size / 2);
	case 1:
		return to_value;
	case -1:
		return to_value - (position + size);
	default:
		return original;
	}
}

void align(sf::Text& item, float x_align, int x_align_type, float y_align, int y_align_type) {
	auto bounds = item.getLocalBounds();
	auto pos = item.getPosition();
	item.setPosition(align(x_align, x_align_type, bounds.left, bounds.width, pos.x),
		align(y_align, y_align_type, bounds.top, bounds.height, pos.y));
}

void align(sf::Text& item, sf::Vector2f align_to, int x_align_type, int y_align_type) {
	align(item, align_to.x, x_align_type, align_to.y, y_align_type);
}

sf::Text* _game_data::player_score(bool top_player) {
	auto result = new sf::Text();
	result->setFont(thick_font);
	result->setCharacterSize(40 * font_multiplier);
	result->setFillColor(top_player ? p1_color : p2_color);
	result->setString("x0");
	if (top_player)
		align(*result, 0, 2, p1_y - p_score_gap, -1);
	else align(*result, 0, 2, p2_y + p_score_gap, 1);
	return result;
}

std::vector<sf::Text>* _game_data::game_over_text() {
	// khỏi tạo chữ và đặt vị trí sẵn cho nó
	sf::Text player_name, win, press_key;
	win.setFont(thin_font);
	win.setCharacterSize(50 * font_multiplier);
	win.setString("PLAYER WINS");
	align(win, play_pos_center, 0, 0);
	auto win_bounds = win.getGlobalBounds();

	player_name.setFont(thick_font);
	player_name.setCharacterSize(100 * font_multiplier);
	player_name.setString("PLAYER");
	align(player_name, play_pos_center.x, 0, win_bounds.top - 30, -1);

	press_key.setFont(thick_font);
	press_key.setCharacterSize(24 * font_multiplier);
	press_key.setString("PRESS SPACE TO REPLAY");
	align(press_key, play_pos_center.x, 0, win_bounds.top + win_bounds.height + 30, 1);

	auto result = new std::vector<sf::Text>();
	result->push_back(player_name);
	result->push_back(win);
	result->push_back(press_key);
	return result;
}

// khởi tạo nút ấn cho menu
void _game_data::init_menu_item(sf::Text& item, const std::string& name) {
	item.setFont(game_data->thin_font);
	item.setCharacterSize(28 * font_multiplier);
	item.setString(name);
}
std::vector<sf::Text>* _game_data::main_menu_text() {
	sf::Text welcome, team_name, name, pvp, pvc, fullscreen, quit;
	name.setFont(thick_font);
	name.setCharacterSize(72 * font_multiplier);
	name.setString("PONG GAME");
	align(name, play_pos_center, 0, 0);
	auto anchor = name.getGlobalBounds();

	welcome.setFont(thick_font);
	welcome.setCharacterSize(30 * font_multiplier);
	welcome.setString("WELCOME TO");
	align(welcome, play_pos_center.x, 0, anchor.top - 30, -1);
	auto welcome_anchor = welcome.getGlobalBounds();

	team_name.setFont(cursive_font);
	team_name.setCharacterSize(60 * font_multiplier);
	team_name.setString("Team SFML");
	align(team_name, play_pos_center.x, 0, welcome_anchor.top - 10, -1);

	init_menu_item(pvp, "Player vs Player");
	align(pvp, game_data->play_pos_center.x, 0, anchor.top + anchor.height + 20, 1);
	anchor = pvp.getGlobalBounds();

	init_menu_item(pvc, "Player vs Computer");
	align(pvc, game_data->play_pos_center.x, 0, anchor.top + anchor.height + 5, 1);
	anchor = pvc.getGlobalBounds();

	init_menu_item(fullscreen, this->fullscreen ? "Window" : "Fullscreen");
	align(fullscreen, anchor.left, 1, anchor.top + anchor.height + 5, 1);

	init_menu_item(quit, "Quit");
	align(quit, anchor.left + anchor.width, -1, anchor.top + anchor.height + 5, 1);

	auto result = new std::vector<sf::Text>();
	result->push_back(pvp);
	result->push_back(pvc);
	result->push_back(name);
	result->push_back(fullscreen);
	result->push_back(quit);
	result->push_back(welcome);
	result->push_back(team_name);
	return result;
}

std::vector<sf::Text>* _game_data::pause_menu_text() {
	sf::Text paused, restart, back, press_key;

	paused.setFont(cursive_font);
	paused.setCharacterSize(80 * font_multiplier);
	paused.setString("Paused");
	align(paused, play_pos_center, 0, -1);
	auto anchor = paused.getGlobalBounds();

	init_menu_item(restart, "Restart");
	align(restart, anchor.left, 1, anchor.top + anchor.height + 20, 1);

	init_menu_item(back, "Back");
	align(back, anchor.left + anchor.width, -1, anchor.top + anchor.height + 20, 1);
	anchor = back.getGlobalBounds();

	press_key.setFont(thick_font);
	press_key.setCharacterSize(20 * font_multiplier);
	press_key.setString("PRESS SPACE TO UNPAUSE");
	align(press_key, play_pos_center.x, 0, anchor.top + anchor.height + 20, 1);

	auto result = new std::vector<sf::Text>();
	result->push_back(restart);
	result->push_back(back);
	result->push_back(paused);
	result->push_back(press_key);
	return result;
}


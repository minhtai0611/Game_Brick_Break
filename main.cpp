#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include "pong_game.h"
#include "player.h"
void start_game(sf::RenderWindow*& window, pong_game*& game) {
	// mở màn hình game
	sf::ContextSettings settings;
	// đặt đồ anti-alias là 4 cho đồ họa đẹp hơn
	settings.antialiasingLevel = 4;
	if (game_data->fullscreen)
	{
		window = new sf::RenderWindow(sf::VideoMode(0, 0), "Pong game!", sf::Style::Fullscreen, settings);
		window->setFramerateLimit(60);
		window->setVerticalSyncEnabled(true);
	}
	else window = new sf::RenderWindow(sf::VideoMode(800, 600), "Pong game!", sf::Style::Default, settings);

	// đặt cửa sổ cho game_data
	game_data->set_window(*window);
	// tạo một game mới
	game = new pong_game(*window);
}
int main() {
	// bật nhạc cho game
	auto music = new sf::Music();
	if (!music->openFromFile("D:\stay inside me theme.ogg"))
		throw;
	music->setLoop(true);
	music->play();

	// khỏi tạo game
	init_game_data();
	sf::RenderWindow* window;
	pong_game* game;
	start_game(window, game);

	while (window->isOpen()) {
		// nếu người dùng bấm tắt màn hình thì thoát luôn
		sf::Event event{};
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
		}

		// xóa màn hình
		window->clear();
		// update game
		if (game->update()) {
			window->close();
			delete(window);
			delete(game);
			game_data->fullscreen = !game_data->fullscreen;
			start_game(window, game);
			continue;
		}
		// hiển thị màn hình
		window->display();
	return 0;
}
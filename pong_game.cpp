//
// Created by quan on 26/10/2019.
//

#include "pong_game.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
pong_game* pong_game;


void pong_game::reset() {
	p1.reset();
	p2->reset();
	b.reset_all(ucontext);
}

bool pong_game::update() {
	// update trạng thái
	ucontext.update();

	if (state != state_paused) {
		// update người chơi 1
		p1.update(ucontext);
		// nếu người chơi 2 khác null thì update luôn
		if (p2)
			p2->update(ucontext);
	}
	p1.draw(ucontext.window);
	if (p2)
		p2->draw(ucontext.window);

	// lấy vị trí chuột
	auto mouse_pos = ucontext.window.mapPixelToCoords(sf::Mouse::getPosition(ucontext.window));
	// kiểm tra có nhấn chuột không
	bool mouse_press = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	// update biến thời gian
	wait_time -= ucontext.elapsed_time;
	// nếu game over thì hiện menu game over và chuẩn bị bắt đầu chơi lại hoặc trở về menu chính
	if (state == state_game_over) {
		// vẽ các dòng trong menu
		ucontext.window.draw(game_over_text[0]);
		ucontext.window.draw(game_over_text[1]);
		// dòng cuối cùng thì có hiệu ứng nhấp nháy
		if (blinking_state())
			ucontext.window.draw(game_over_text[2]);
		// nếu ấn phím cách thì bắt đầu chơi, ấn Esc thì trở về menu chính
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			state = state_normal;
			reset();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			state = state_main_menu;
			reset();
		}

	}
	else
		// nếu trạng thái state_main_menu thì hiện menu chính và chuẩn bị bắt đầu chơi
		if (state == state_main_menu) {
			// dòng chữ tên game thì có hiệu ứng nhấp nháy màu
			main_menu_text[2].setFillColor(blinking_state() ? game_data->p1_color : game_data->p2_color);

			// vẽ menu
			for (auto text : main_menu_text)
				ucontext.window.draw(text);
			// nếu ấn nút pvp (player vs player) thì đặt p2 là người, đặt tên người chơi phù hợp và chơi
			if (pvp_btn.update(mouse_pos) && mouse_press) {
				state = state_normal;
				main_menu_text[2].setFillColor(sf::Color(96, 96, 96));
				p2 = &p2_human;
				p1.name = "TOP";
				p2->name = "BOTTOM";
			}
			// nếu ấn nút pvc (player vs computer) thì đặt p2 là máy
			else if (pvc_btn.update(mouse_pos) && mouse_press) {
				state = state_normal;
				main_menu_text[2].setFillColor(sf::Color(96, 96, 96));
				p2 = &p2_computer;
				p1.name = "HOOMAN";
				p2->name = "AI";
			}
			// nếu ấn nút quit thì thoát game
			else if (quit_btn.update(mouse_pos) && mouse_press) {
				ucontext.window.close();
			}

			else if (fullscreen_btn.update(mouse_pos) && mouse_press) {
				return true;
			}
		}
	// hiện menu tạm dừng
		else if (state == state_paused) {
			// nếu ấn nút back thì gán state bằng state_main_menu để quay lại menu chính
			if (back_btn.update(mouse_pos) && mouse_press) {
				state = state_main_menu;
				reset();
			}
			// nếu ấn restart thì reset game rồi
			else if (restart_btn.update(mouse_pos) && mouse_press) {
				reset();
				state = state_normal;
			}
			// nấu ấn phím cách thì tiếp tục chơi bình thường
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				state = state_normal;
			// ngược lại thì vẽ menu
			else {
				// dòng chữ tên game thì có hiệu ứng nhấp nháy màu
				paused_text[2].setFillColor(blinking_state() ? game_data->p1_color : game_data->p2_color);
				ucontext.window.draw(paused_text[0]);
				ucontext.window.draw(paused_text[1]);
				ucontext.window.draw(paused_text[2]);
				// dòng cuối cùng sẽ nhấp nháy
				if (blinking_state())
					ucontext.window.draw(paused_text[3]);
			}
		}
	// ngược lại, nếu đang chơi game thì update banh và vẽ tên game giữa màn hình
	// trong các menu, người chơi vấn có thể điểu khiện player của mình, nhưng sẽ không có banh để chơi
		else {

			// vẽ tên game làm background cho sân chơi
			ucontext.window.draw(main_menu_text[2]);
			// update banh
			b.update(ucontext, p1, *p2);
			// nếu ấn escape thì tạm dừng chơi
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				state = state_paused;
		}
	if (state == state_normal || state == state_paused)
		b.draw(ucontext.window);
	return false;
}


pong_game::pong_game(sf::RenderWindow& window) :
	// khởi tạo biến trạng thái
	ucontext(window, *this),
	// yêu cầu các menu từ game_data
	game_over_text(*game_data->game_over_text()), main_menu_text(*game_data->main_menu_text()), paused_text(*game_data->pause_menu_text()),
	// khởi tạo các người chơi
	p1(true, ucontext), p2(nullptr), p2_human(false, ucontext), p2_computer(false, ucontext, *this),
	// khởi tạo trái banh
	b(ucontext),
	pvp_btn(main_menu_text[0], game_data->p1_color), pvc_btn(main_menu_text[1], game_data->p2_color),
	fullscreen_btn(main_menu_text[3], game_data->p1_color), quit_btn(main_menu_text[4], game_data->p2_color),
	restart_btn(paused_text[0], game_data->p1_color), back_btn(paused_text[1], game_data->p2_color) {
	// đặt seed cho random
	srand(time(nullptr));
	// đi vào menu chính
	state = state_main_menu;
	// đặt p2 là người chơi, để có thể điều khiển cả 2 player trong menu chính
	p2 = &p2_human;
}

void pong_game::game_over(player& losing_player) {
	// đặt trang thái state_game_over
	state = state_game_over;
	// xác định người thắng cuộc và hiện tên người đó lên menu
	if (losing_player.top_player) {
		// nếu người thua là người chơi trên, vậy người thắng là người chơi dưới (p2)
		// ghi tên họ vào menu
		game_over_text[0].setString(p2->name);
		// đặt màu của họ cho menu
		game_over_text[0].setFillColor(game_data->p2_color);
	}
	else {
		// tương tự với p1
		game_over_text[0].setString(p1.name);
		game_over_text[0].setFillColor(game_data->p1_color);
	}
	// canh giữa dòng tên người chơi (do vừa thay đổi nội dung)
	// các dòng khác không bị thay đổi nên không cần canh
	align(game_over_text[0], game_data->play_pos_center.x, 0, 0, 2);
	wait_time = 0;
}

bool pong_game::blinking_state() {
	return (int)(-wait_time * 2) % 2 == 0;
}
void pong_game::play_sound(Sound& s, float pitch)
{
	float p = s.getPitch();
	s.setPitch(pitch);
	s.play();
}
bool pong_game::list_sound() {//Danh sach nhac
	VideoMode video_mode(320, 240);//khung hinh cho video nhac
	RenderWindow window(video_mode, "Piano");
	SoundBuffer soundBuffer1;//Nhac duoc dinh nghia tu file 
	if (!soundBuffer1.loadFromFile("blip.wav"))
		return EXIT_FAILURE;//tra ve gia tri 1
	Sound blip(soundBuffer1);//mo nhac

	SoundBuffer soundBuffer2;
	if (!soundBuffer2.loadFromFile("blam.wav"))
		return EXIT_FAILURE;
	Sound blam(soundBuffer2);

	SoundBuffer soundBuffer3;
	if (!soundBuffer3.loadFromFile("blap.wav"))
		return EXIT_FAILURE;
	Sound blap(soundBuffer3);

	SoundBuffer soundBuffer4;
	if (!soundBuffer4.loadFromFile("blop.wav"))
		return EXIT_FAILURE;
	Sound blop(soundBuffer4);

	Font font;
	if (!font.loadFromFile("tomb.ttf"))
		return EXIT_FAILURE;

	Text info("Use the keys \n A Z E R \n Q S D F\n W X C V\n to play sounds.", font, 20);
	info.setPosition(20, 30);
	info.setColor(Color::Blue);

	while (window.isOpen())
	{
		window.clear();
		window.draw(info);
		window.display();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			else
				if (event.type == Event::KeyPressed)
				{
					switch (event.key.code)
					{
					case Keyboard::Q:
						play_sound(blip, 1.0);
						break;
					case Keyboard::S:
						play_sound(blam, 1.0);
						break;
					case Keyboard::D:
						play_sound(blap, 1.0);
						break;
					case Keyboard::F:
						play_sound(blop, 1.0);
						break;

					case Keyboard::W:
						play_sound(blip, 0.8);
						break;
					case Keyboard::X:
						play_sound(blam, 0.8);
						break;
					case Keyboard::C:
						play_sound(blap, 0.8);
						break;
					case Keyboard::V:
						play_sound(blop, 0.8);
						break;

					case Keyboard::A:
						play_sound(blip, 1.2);
						break;
					case Keyboard::Z:
						play_sound(blam, 1.2);
						break;
					case Keyboard::E:
						play_sound(blap, 1.2);
						break;
					case Keyboard::R:
						play_sound(blop, 1.2);
						break;

					case Keyboard::Escape:window.close();
						break;

					}
				}
		}

	}
	return EXIT_SUCCESS;//tra ve gia tri 0
}
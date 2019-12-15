//
// Created by quan on 26/10/2019.
//

#ifndef CTDL_PROJECT01_PONG_GAME_H
#define CTDL_PROJECT01_PONG_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "player.h"
#include "game_data.h"
#include "player_human.h"
#include "player_computer.h"
#include "ball.h"
#include "button.h"
using namespace sf;
class update_context;

class pong_game : ball_game {
	// trạng thái của game
	update_context ucontext;
	// người chơi 1 và người chơi 2 (nếu cho player vs player)
	player_human p1, p2_human;
	// người chơi 2 (nếu chọn player vs computer)
	player_computer p2_computer;
	// người chơi 2 hiện tại (có thể trỏ tới p2_human hoặc p2_computer)
	player* p2;
	// dùng để tạo hiệu ứng nhấp nháy
	bool blinking_state();
public:
	// trái banh đề chơi
	ball b;
	// thời gian của game (dùng cho hiệu ứng nhấp nháy)
	float wait_time;
	// dùng để hiển thị menu game over, menu chính và menu tạm dừng
	std::vector<sf::Text> game_over_text, main_menu_text, paused_text;
	// class button để quản lý các nút trong game
	button pvp_btn, pvc_btn, fullscreen_btn, quit_btn, restart_btn, back_btn;

	// trạng thái của game
	// state_main_menu: đang ở menu chính
	// state_normal: đang chơi
	// state_game_over: đã kết thúc ván chơi, đang đợi chơi lại hoặc trở về menu
	// state_paused: đang tạm dừng trò chơi
	int state;
	static const int state_normal = 0, state_game_over = 1, state_main_menu = 2, state_paused = 3;

	// tạo game mới từ cửa số window
	explicit pong_game(sf::RenderWindow& window);

	// update game theo vòng lặp
	// trả về true nếu game yêu cầu đổi chế độ hiển thị (giữa fullscreen và window)
	bool update();

	// đi vào trạng thái state_game_over (kết thúc game)
	// losing_player: người chơi thua trong ván chơi
	void game_over(player& losing_player) override;

	// reset game (bắt đầu ván chơi mới)
	void reset();
	void play_sound(Sound& s, float pitch);//them nhac cho game
	bool list_sound();
};

#endif //CTDL_PROJECT01_PONG_GAME_H
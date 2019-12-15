//
// Created by quan on 27/10/2019.
//

#include "player_human.h"

// nhận input từ bàn phím để điều khiển
void player_human::take_user_input(float& move_dir) {
	if (sf::Keyboard::isKeyPressed(positive_ctrl))
		move_dir++;
	else if (sf::Keyboard::isKeyPressed(negative_ctrl))
		move_dir--;
}

player_human::player_human(bool top_player, update_context& context) : player(top_player, context) {
	if (this->top_player) {
		// đặt điểu khiển của người chơi
		positive_ctrl = game_data->p1_negative;
		negative_ctrl = game_data->p1_positive;
	}
	else {
		positive_ctrl = game_data->p2_negative;
		negative_ctrl = game_data->p2_positive;
	}
}
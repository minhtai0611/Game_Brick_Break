//
// Created by quan on 27/10/2019.
//

#include "player_computer.h"
#include "pong_game.h"

void player_computer::take_user_input(float& move_dir) {
	// đặt hướng đi về phía trái banh
	move_dir = out_of_bounds(position.x - game.b.position.x, -0.5f, 0.5f);
}

player_computer::player_computer(bool top_player, update_context& context, pong_game& game) : player(top_player, context), game(game) {

}
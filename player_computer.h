//
// Created by quan on 27/10/2019.
//

#ifndef OOP_PROJECT01_PLAYER_COMPUTER_H
#define OOP_PROJECT01_PLAYER_COMPUTER_H


#include "player.h"

class pong_game;

// máy sẽ luôn di chuyển về phía trái banh
class player_computer : public player {
protected:
	void take_user_input(float& move_dir) override;
	// cần có reference đến game để xác định vị trí trái banh
	pong_game& game;

public:
	player_computer(bool top_player, update_context& context, pong_game& game);
};


#endif //OOP_PROJECT01_PLAYER_COMPUTER_H
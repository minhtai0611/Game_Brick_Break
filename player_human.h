//
// Created by quan on 27/10/2019.
//

#ifndef OOP_PROJECT01_PLAYER_HUMAN_H
#define OOP_PROJECT01_PLAYER_HUMAN_H


#include "player.h"

class player_human : public player {
protected:
	// các phím điều khiển phải trái
	sf::Keyboard::Key positive_ctrl, negative_ctrl;

	void take_user_input(float& move_dir) override;

public:
	player_human(bool top_player, update_context& context);
};


#endif //OOP_PROJECT01_PLAYER_HUMAN_H
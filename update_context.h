//
// Created by quan on 26/10/2019.
//

#ifndef CTDL_PROJECT01_UPDATE_CONTEXT_H
#define CTDL_PROJECT01_UPDATE_CONTEXT_H

#include <SFML/Graphics.hpp>
#include "game_data.h"
#include<time.h>
class player;

// interface cho trò chơi, chửa void game_over để gọi báo kết thúc trò chơi
struct ball_game {
	virtual void game_over(player& losing_player) = 0;
};

class update_context {
public:
	// clock để đo thời gian đã trôi qua giữa hai update
	sf::Clock time;
	// cửa sổ hiện tại
	sf::RenderWindow& window;
	// game hiện tại
	ball_game& game;
	// thời gian đã trôi qua giữa update
	float elapsed_time;

	// khởi tạo từ cửa sổ window và game 
	explicit update_context(sf::RenderWindow& window, ball_game& game);

	// cập nhật elapsed_time
	void update();
	int exec();
};


#endif //CTDL_PROJECT01_UPDATE_CONTEXT_H
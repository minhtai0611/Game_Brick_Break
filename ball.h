//
// Created by quan on 26/10/2019.
//

#ifndef CTDL_PROJECT01_BALL_H
#define CTDL_PROJECT01_BALL_H


#include "update_context.h"
#include "player.h"

class ball {
private:
	// làm banh bắt đầu di chuyển (bắt đầu ván chơi)
	void start_moving();

	// Lặp lại hiệu ứng quay cho banh
	// đặt tốc độ xoay về giá trị ban đầu
	// đổi hướng xoay của banh
	void reset_spin();

public:

	// Danh sách các hình để vẽ banh
	std::vector<sf::Shape*> shapes;

	// Vị trí và vận tốc của banh 
	sf::Vector2f position, velocity;
	// góc hiện tại của banh
	float angle;
	// vận tốc ban đầu của banh
	float speed;
	// tốc độ xoay hiện tại 
	float spin;
	// tốc độ xoay lúc bắt đầu hiệu ứng quay
	float spin_max;
	// tốc độ xoay bị mất đi mỗi giây
	float spin_decrease;
	// hướng xoay
	int spin_dir;
	// thời gian dừng mỗi khi bắt đầu và kết thúc lượt chơi
	float start_delay;
	// thời gian đợi còn lại
	float wait_time;
	// trạng thái của banh
	// state_restart: chuẩn bị bắt đầu chơi, banh nằm ở giữa màn hình đợi bắt đầu
	// state_normal: trạng thái bình thường, banh đang di chuyển
	// state_over: một người chơi không bắt được banh, banh đang bay ra ngoài màn hình
	int ball_state;
	static const int state_normal = 0, state_over = 1, state_restart = 2;

	// khỏi tạo banh từ object trạng thái
	// context: trường ucontext của pong_game hiện tại
	explicit ball(update_context& context);

	// cho banh trở về trạng thái ban đầu (bắt đầu ván chơi mới)
	void reset_all(update_context& context);

	// cập nhật trạng thái banh
	void update(update_context& context, player& p1, player& p2);

	// vẽ banh
	void draw(sf::RenderWindow& window);

	// tăng tốc banh (mỗi khi bật ra khỏi người chơi)
	void accelerate();
};


#endif //CTDL_PROJECT01_BALL_H
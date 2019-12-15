//
// Created by quan on 26/10/2019.
//

#include <cmath>
#include "ball.h"

ball::ball(update_context& context) :
	// yêu cầu graphic của trái banh từ singleton game_data
	shapes(*game_data->ball_shape()) {
	// yêu cầu các biến từ game_data
	speed = game_data->b_speed;
	start_delay = game_data->delay_time;
	// trở về trạng thái ban đầu của game
	reset_all(context);
	// ở lượt chơi đầu tiên, đợi lâu hơn cho người dùng chuẩn bị
	wait_time *= 2;
	
}

void ball::reset_all(update_context& context) {
	// đặt lại các biến trạng thái, cho banh đứng yên giữa màn hình
	spin = 0;
	angle = 0;
	velocity = sf::Vector2f(0, 0);
	spin_dir = 1;
	position = game_data->play_pos_center;
	// đặt lại tốc độ xoay
	spin_max = game_data->b_spin;
	spin_decrease = game_data->b_spin_decrease;
	// chuyển sang trạng thái state_restart chuẩn bị chơi
	ball_state = state_restart;
	wait_time = start_delay;
}

// đưa về vector có độ dài là 1
sf::Vector2f normalize(sf::Vector2f source) {
	float magnitude = std::sqrt(source.x * source.x + source.y * source.y);
	return source / magnitude;
}

// trả về giá trị 1 hoặc -1 (ngẫu nhiên)
int rand_dir() {
	return rand() % 2 ? -1 : 1;
}

void ball::start_moving() {
	reset_spin();
	velocity = normalize(sf::Vector2f(rand_dir(), rand_dir())) * speed;
	ball_state = state_normal;
}

void ball::reset_spin() {
	spin = spin_max;
	spin_dir = -spin_dir;
}

void ball::update(update_context& context, player& p1, player& p2) {
	// góc = góc + tốc độ quay * thời gian trôi qua
	angle = angle + spin * spin_dir * context.elapsed_time;
	// giảm tốc độ quay từ từ cho đến 0
	spin = std::fmax(0.f, spin - spin_decrease * context.elapsed_time);

	// thay đổi vị trí banh
	position += velocity * context.elapsed_time;
	// nếu trong chế độ bình thường thì kiểm tra banh có ra khỏi biên (còn không thì cho nó bay ra ngoài luôn)
	if (ball_state == state_normal) {
		// kiểm tra nếu ra khỏi biên dọc thì đảo ngược tốc độ ngang cho banh bật lại
		if (game_data->out_of_bounds_x(position.x, game_data->b_size)) {
			velocity.x = -velocity.x;
			// đưa banh trở lại vào trong biên để tránh bị nhận dạng ở ngoài biên một lần nữa
			position.x = game_data->coerce_in_x(position.x, game_data->b_size);
			// chạy lại hiệu ứng quay khi banh bật lại
			reset_spin();
		}
		// kiểm tra biên ngang: out_of_bounds_y trả về -1 tức là ra ngoài phía trên, 1 tức là ra ngoài phía dưới
		// 0 tức là không ra ngoài
		int bounds_y = game_data->out_of_bounds_y(position.y, game_data->b_size);
		if (bounds_y != 0) {
			// xác định người chơi cần phải hứng banh 
			player& p = bounds_y == -1 ? p1 : p2;
			// nếu banh ra ngoài thì báo với người chơi đó để tự trừ điểm
			if (std::abs(position.x - p.position.x) > game_data->p_size + game_data->b_size) {
				// nêu lost_point trả về true (mất hết điểm) thì game_over()
				if (p.lose_point())
					context.game.game_over(p);
				// chuyển sang trạng thái state_over
				ball_state = state_over;
				wait_time = start_delay;
				// báo cho cả hai người chơi đẻ kết thúc lượt chơi, trở về vị trí ban đầu
				p1.over();
				p2.over();
			}
			else
				// nếu hứng thành công thi cho banh bật lại
			{
				velocity.y = -velocity.y;
				position.y = game_data->coerce_in_y(position.y, game_data->b_size);
				reset_spin();
				// tăng tốc banh để tạo độ khó
				accelerate();
			}
		}
	}
	else {
		// thời gian còn lại trừ đi thời gian đã trôi qua
		wait_time -= context.elapsed_time;
		// nêu hết thời gian còn lại thì bắt đầu di chuyển
		if (wait_time < 0) {
			// nếu trong trạng thái state_restart thì đọi hết thời gian rồi cho banh di chuyển
			if (ball_state == state_restart)
				start_moving();
			// nếu trong trạng thái state_over, kết thúc game, đợi hết thời gian rồi reset lại banh
			else if (ball_state == state_over)
				reset_all(context);
		}
	}

}

void ball::accelerate() {
	// tăng tốc độ di chuyển cũng như tốc độ quay
	velocity.x *= 1.1f;
	velocity.y *= 1.1f;
	spin_max *= 1.1f;
	spin_decrease *= 1.1f;
}

void ball::draw(sf::RenderWindow& window) {
	// vẽ các bộ phận của banh
	for (auto shape : shapes) {
		shape->setRotation(angle);
		shape->setPosition(position);
		window.draw(*shape);
	}
}
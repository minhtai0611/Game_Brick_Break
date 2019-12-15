//
// Created by quan on 26/10/2019.
//

#include <iostream>
#include <cmath>
#include "player.h"
#include "pong_game.h"

// khởi tạo player
player::player(bool top_player, update_context& context) :
	// yêu cầu các shape và điểm hiển thị từ game_data
	shapes(*game_data->player_shape(top_player)),
	score_text(*game_data->player_score(top_player)) {
	speed = 0;
	this->top_player = top_player;
	// reset để bắt đầu game mới
	reset();
	if (this->top_player) {
		// đặt vị trí của người chơi
		position = sf::Vector2f(game_data->play_pos_center.x, game_data->p1_y);
		// nếu là người chơi trên thì còn phải đảo ngược các shape
		for (auto shape : shapes)
			shape->setScale(1, -1);
	}
	else {
		position = sf::Vector2f(game_data->play_pos_center.x, game_data->p2_y);
	}
}
// update player
void player::update(update_context& context) {
	float move_dir = 0;
	// nếu ở đang chơi game thì nhận input từ class con bình thường
	if (state == state_normal)
		take_user_input(move_dir);
	else {
		// nếu không thì...
		// trước hết kiểm tra đã hết thời gian đợi chưa để còn chuyển sang chế độ khác
		wait_time -= context.elapsed_time;
		if (wait_time < 0) {
			switch (state) {
			case state_over:
				state = state_reset;
				wait_time = game_data->delay_time;
				break;
			case state_reset:
				state = state_normal;
				break;
			}
		}
		// sau đó, nếu mởi kết thúc lượt chưa lâu hoặc sắp bắt đầu lượt thì lại nhận input từ class con
		if ((state == state_over && wait_time > game_data->delay_time * 0.5f) ||
			(state == state_reset && wait_time < game_data->delay_time * 0.5f))
			take_user_input(move_dir);
		else {
			// ngược lại, di chuyển người chơi về giữa màn hình để chuẩn bị ván chơi mới
			move_dir = out_of_bounds(position.x - game_data->play_pos_center.x, -0.5f, 0.5f);
		}
	}
	// nếu hướng điều khiển ngược với hướng di chuyển thì gán lại di chuyển theo hướng điều khiển
	// vận tốc sẽ trở về nhỏ nhất
	if (move_dir * speed <= 0)
		speed = game_data->p_min_speed * move_dir;
	// ngược lại thì tăng tốc di chuyển cho đến vận tốc lớn nhất
	else speed = coerce_in(speed + game_data->p_accel * context.elapsed_time * move_dir, game_data->p_max_speed);
	// người chơi có thể di chuyển ra ngoài sân chơi một tí, đây là kích thước người chơi bắt buộc phải ở trong sân
	auto p_min_size = game_data->p_size - game_data->p_thickness;
	// giới hạn vị trí người chơi ở trong sân
	position.x = game_data->coerce_in_x(position.x - speed * context.elapsed_time, p_min_size);

}

// giảm đi 1 điểm, nếu hết điểm thì trả về true
bool player::lose_point() {
	set_score(get_score() - 1);
	return get_score() == 0;
}
// gán điểm và thay đổi hiển thị của nó trên màn hình
void player::set_score(int value) {
	// gán giá trị cho điểm
	_score = value;
	// hiển thị điểm mới trên màn hình
	score_text.setString(std::to_string(value));
	// canh giữa lại điểm trên màn hình
	align(score_text, game_data->play_pos_center.x, 0, 0, 2);
}

int player::get_score() {
	return _score;
}

void player::over() {
	// đặt trạng thái thành state_over
	state = state_over;
	// đặt thời gian đợi trong trạng thái này
	wait_time = game_data->delay_time;
}

void player::reset() {
	// đặt lại điểm người chơi
	set_score(game_data->p_score);
	// trở về trang thái state_normal
	state = state_reset;
	wait_time = game_data->delay_time;
}

void player::draw(sf::RenderWindow& window) {
	// vẽ người chơi lên màn hình
	for (auto shape : shapes) {
		shape->setPosition(position);
		window.draw(*shape);
	}
	// hiển thị điểm số
	window.draw(score_text);
}
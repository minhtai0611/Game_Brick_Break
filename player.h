//
// Created by quan on 26/10/2019.
//

#ifndef CTDL_PROJECT01_PLAYER_H
#define CTDL_PROJECT01_PLAYER_H

#include "update_context.h"

class player {
protected:
	// điểm hiện tại của người chơi
	int _score;
	// nhận điều khiển của cho player (có thể từ người chơi hoặc từ máy)
	// move_dir: biến tiếp nhận điều khiển của player (-1: đi về bên trái, 1: đi về bên phải, 0: không di chuyển)
	virtual void take_user_input(float& move_dir) = 0;

public:

	// các shape để vẽ người chơi
	std::vector<sf::Shape*> shapes;
	// hiển thị điểm của người chơi
	sf::Text score_text;
	// vị trí của người chơi
	sf::Vector2f position;
	// tốc độ hiện tại của người chơi
	// khi bắt đầu di chuyển, tốc độ sẽ bằng game_date->p_min_speed
	// rồi tăng dần với tốc độ p_accel cho đến khi đạt tốc độ tối đa p_max_speed
	float speed;
	// thời gian đợi các sự kiện của người chơi
	float wait_time;
	// có là người chơi phía trên hay không
	bool top_player;
	// trạng thái của người chơi
	// tương ứng với trạng thái của banh
	int state;
	static const int state_normal = 0, state_over = 1, state_reset = 2;
	// tên hiển thị của người chơi
	std::string name;

	// tạo người chơi từ context
	// top_player cho biết cần phải tạo người chơi ở phía trên hay phía
	player(bool top_player, update_context& context);
	// cho người chơi mất điểm
	bool lose_point();
	// báo người chơi rằng lượt chơi đã kết thúc
	void over();
	// cập nhật trạng thái người chơi
	void update(update_context& context);
	// vẽ người chơidưới lên cửa sổ window
	void draw(sf::RenderWindow& window);
	// gán giá trị cho điểm và thay đổi điểm được hiện trên màn hình
	void set_score(int value);
	// trả về điểm người chơi
	int get_score();

	void reset();
};

#endif //CTDL_PROJECT01_PLAYER_H
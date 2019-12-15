//
// Created by quan on 26/10/2019.
//

#include "update_context.h"

update_context::update_context(sf::RenderWindow& window, ball_game& game) : window(window), game(game) {
	time = sf::Clock();
	elapsed_time = 0.4f;
	time.getElapsedTime();

}

void update_context::update() {
	elapsed_time = time.restart().asSeconds();
}
int update_context::exec()
{
	Clock renderClock, updateClock;
	while (window.isOpen())
	{
		time = renderClock.getElapsedTime();
		float fFps = 1000000 / time.asMicroseconds();
		std::stringstream s;
		s << fFps;
		fps.setString(s.str());
		renderClock.restart();

		const Int64 frameTime = 1000000 / FRAMES_PER_SECOND;
		Clock c;
		Time t = c.getElapsedTime();
		Int64 nextFrameTime = t.asMicroseconds() + frameTime;

		int loops = 0;
		while (t.asMicroseconds() < nextFrameTime && loops < MAX_FRAMESKIP)
		{
			processEvents();
			updateTime = updateClock.restart().asMilliseconds();
			update();
			t = c.getElapsedTime();
			loops++;
		}

		display();
	}

#pragma once

#include "Renderer.h"


class Application {
private:
	sf::RenderWindow* m_window;
	sf::Clock m_clock;

	Player m_player;
	Level m_level;
	Renderer* m_renderer;

private:
	void initialize();
	
	void handleInputs();
	void update();
	void render();

public:
	Application();
	~Application();

	void run();
};

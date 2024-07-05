#pragma once

#include <SFML/Graphics.hpp>


class Application {
private:
	sf::RenderWindow* m_window;
	sf::Clock m_clock;

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

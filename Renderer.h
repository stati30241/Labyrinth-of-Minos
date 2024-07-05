#pragma once

#include "Level.h"


class Player {
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::Vector2f cameraPlane;
};


class Renderer {
protected:
	sf::RenderWindow* m_window;

public:
	Renderer(sf::RenderWindow* window, const Level* level);

	virtual void render(const Player& player) = 0;
};


class MiniMapRenderer : public Renderer {
private:
	sf::VertexArray m_levelVertecies{ sf::Quads };

private:
	void initialize(const Level* level);

public:
	MiniMapRenderer(sf::RenderWindow* window, const Level* level);

	virtual void render(const Player& player) override;
};

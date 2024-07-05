#pragma once

#include "Level.h"


class Player {
private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_direction;

public:
	Player() { };
	Player(const sf::Vector2f& position, const sf::Vector2f& direction)
		: m_position{ position }, m_velocity{ }, m_direction{ direction } {

	}

	void setPosition(const sf::Vector2f& position) { m_position = position; }
	void move(const sf::Vector2f& offset) { m_position += offset; }
	void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
	void accelerate(const sf::Vector2f& offset) { m_velocity += offset; }
	void setDirection(const sf::Vector2f& direction) { m_direction = direction; }

	sf::Vector2f getPosition() const { return m_position; }
	sf::Vector2f getVelocity() const { return m_velocity; }
	sf::Vector2f getDirection() const { return m_direction; }
	sf::Vector2f getCameraPlane() const { return sf::Vector2f{ -m_direction.y, m_direction.x }; }
};


class Renderer {
protected:
	sf::RenderWindow* m_window;
	Level* m_level;

protected:
	bool dda(const sf::Vector2f& start, const sf::Vector2f& dir, sf::Vector2f& intersection);

public:
	Renderer(sf::RenderWindow* window, Level* level);

	virtual void render(const Player& player) = 0;
};


class MiniMapRenderer : public Renderer {
private:
	sf::Vector2f m_tileSize;
	sf::VertexArray m_levelVertecies{ sf::Quads };

private:
	void initialize();

public:
	MiniMapRenderer(sf::RenderWindow* window, Level* level);

	virtual void render(const Player& player) override;
};

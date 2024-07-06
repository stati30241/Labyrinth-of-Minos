#pragma once

#include "Level.h"


class Player {
private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_angle = 0.0f;
	float m_fov = 0.0f;

public:
	Player() { };
	Player(const sf::Vector2f& position, float angle, float fov)
		: m_position{ position }, m_velocity{ }, m_angle{ angle }, m_fov{ fov } {

	}

	void setPosition(const sf::Vector2f& position) { m_position = position; }
	void move(const sf::Vector2f& offset) { m_position += offset; }
	void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
	void accelerate(const sf::Vector2f& offset) { m_velocity += offset; }
	void setAngle(float angle) { m_angle = angle; }
	void setFov(float fov) { m_fov = fov; }

	sf::Vector2f getPosition() const { return m_position; }
	sf::Vector2f getVelocity() const { return m_velocity; }
	float getAngle() const { return m_angle; }
	float getFov() const { return m_fov; }
};


class Renderer {
protected:
	struct DDAInfo {
		bool hit;
		sf::Vector2f intersectionPoint;
		float euclideanDist;
		float perpendicularDist;
		bool side; // False is x, true is y
	};

protected:
	sf::RenderWindow* m_window;
	Level* m_level;

protected:
	DDAInfo dda(const sf::Vector2f& start, const sf::Vector2f& dir, float threshold);

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


class RaycastRenderer : public Renderer {
private:
	sf::VertexArray m_screen{ sf::Lines };

public:
	RaycastRenderer(sf::RenderWindow* window, Level* level);

	virtual void render(const Player& player) override;
};

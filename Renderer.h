#pragma once

#include "Level.h"


class Player {
private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_angle;
	float m_fov = 0.0f;

public:
	Player() { };
	Player(const sf::Vector2f& position, const sf::Vector2f& angle, float fov)
		: m_position{ position }, m_velocity{ }, m_angle{ angle }, m_fov{ fov } {

	}

	void setPosition(const sf::Vector2f& position) { m_position = position; }
	void move(const sf::Vector2f& offset) { m_position += offset; }
	void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
	void accelerate(const sf::Vector2f& offset) { m_velocity += offset; }
	void setAngle(sf::Vector2f angle) { m_angle = angle; }
	void setFov(float fov) { m_fov = fov; }

	sf::Vector2f getPosition() const { return m_position; }
	sf::Vector2f getVelocity() const { return m_velocity; }
	sf::Vector2f getAngle() const { return m_angle; }
	float getFov() const { return m_fov; }

	sf::Vector2f getDirection() const { return sf::Vector2f{ std::cosf(m_angle.x), -std::sinf(m_angle.x) }; }
	sf::Vector2f getCameraPlane() const { return std::tanf(m_fov / 2.0f) * sf::Vector2f{ -std::sinf(m_angle.x), -std::cosf(m_angle.x) }; }
};


class Renderer {
private:
	struct DDAInfo {
		bool hit;
		sf::Vector2f intersectionPoint;
		float distance;
		bool side; // False is x, true is y
	};

private:
	sf::RenderWindow* m_window;
	Level* m_level;
	sf::VertexArray m_screen{ sf::Lines };
	sf::Image m_texture;
	sf::Image m_floor;

private:
	DDAInfo dda(const sf::Vector2f& start, const sf::Vector2f& dir, float threshold);

public:
	Renderer(sf::RenderWindow* window, Level* level);

	virtual void render(const Player& player);
};

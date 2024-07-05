#pragma once

#include "GUI.h"

class Level {
private:
	sf::Vector2i m_size;
	std::vector<bool> m_level;

private:
	void generateLevel();

public:
	Level() { };
	Level(const sf::Vector2i& size);

	sf::Vector2i getSize() const { return m_size; }

	bool isTileSolid(const sf::Vector2i& index) const { return m_level.at(index.x + index.y * m_size.x); }
	bool isTileSolid(size_t i, size_t j) const { return m_level.at(i + j * m_size.x); }
};


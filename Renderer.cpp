#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window, Level* level)
	: m_window{ window }, m_level{ level } {

}


bool Renderer::dda(const sf::Vector2f& start, const sf::Vector2f& dir, sf::Vector2f& intersection) {
	const sf::Vector2f unitStepSize = { std::sqrtf(1 + (dir.y / dir.x) * (dir.y / dir.x)),
		std::sqrtf(1 + (dir.x / dir.y) * (dir.x / dir.y)) };
	sf::Vector2i currentTile = sf::Vector2i{ start };
	sf::Vector2f ray, step;

	if (dir.x < 0) {
		step.x = -1.0f;
		ray.x = (start.x - currentTile.x) * unitStepSize.x;
	} else {
		step.x = 1.0f;
		ray.x = (currentTile.x + 1 - start.x) * unitStepSize.x;
	}
	if (dir.y < 0) {
		step.y = -1.0f;
		ray.y = (start.y - currentTile.y) * unitStepSize.y;
	} else {
		step.y = 1.0f;
		ray.y = (currentTile.y + 1 - start.y) * unitStepSize.y;
	}

	bool intersectionFound = false;
	float distance = 0.0f;
	while (!intersectionFound && distance < 5) {
		if (ray.x < ray.y) {
			currentTile.x += step.x;
			distance = ray.x;
			ray.x += unitStepSize.x;
		} else {
			currentTile.y += step.y;
			distance = ray.y;
			ray.y += unitStepSize.y;
		}

		if (currentTile.x >= 0 && currentTile.x < m_level->getSize().x &&
			currentTile.y >= 0 && currentTile.y < m_level->getSize().y) {
			if (m_level->isTileSolid(currentTile)) intersectionFound = true;
		}
	}

	if (intersectionFound) intersection = start + dir * distance;

	return intersectionFound;
}


MiniMapRenderer::MiniMapRenderer(sf::RenderWindow* window, Level* level)
	: Renderer(window, level) {
	initialize();
}


void MiniMapRenderer::initialize() {
	m_tileSize = sf::Vector2f{ m_window->getSize() / m_level ->getSize() };
	for (size_t i = 0; i < m_level->getSize().x; ++i) {
		for (size_t j = 0; j < m_level->getSize().y; ++j) {
			if (!m_level->isTileSolid(i, j)) continue;
			const sf::Vector2f pos = sf::Vector2f{ m_tileSize.x * i, m_tileSize.y * j };
			m_levelVertecies.append({ {pos.x, pos.y}, sf::Color::Green });
			m_levelVertecies.append({ {pos.x + m_tileSize.x, pos.y}, sf::Color::Green });
			m_levelVertecies.append({ {pos.x + m_tileSize.x, pos.y + m_tileSize.y}, sf::Color::Green });
			m_levelVertecies.append({ {pos.x, pos.y + m_tileSize.y}, sf::Color::Green });
		}
	}
}


void MiniMapRenderer::render(const Player& player) {
	sf::CircleShape playerCircle{ 10 };
	playerCircle.setOrigin(10.0f, 10.0f);
	playerCircle.setPosition(player.getPosition() * m_tileSize);

	

	m_window->draw(m_levelVertecies);
	m_window->draw(playerCircle);
}
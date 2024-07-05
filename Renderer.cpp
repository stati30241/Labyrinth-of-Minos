#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window, const Level* level)
	: m_window{ window } {

}


MiniMapRenderer::MiniMapRenderer(sf::RenderWindow* window, const Level* level)
	: Renderer(window, level) {
	initialize(level);
}


void MiniMapRenderer::initialize(const Level* level) {
	m_tileSize = sf::Vector2f{ m_window->getSize() / level->getSize() };
	for (size_t i = 0; i < level->getSize().x; ++i) {
		for (size_t j = 0; j < level->getSize().y; ++j) {
			if (!level->isTileSolid(i, j)) continue;
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
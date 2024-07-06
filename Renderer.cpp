#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window, Level* level)
	: m_window{ window }, m_level{ level } {

}


Renderer::DDAInfo Renderer::dda(const sf::Vector2f& start, const sf::Vector2f& dir, float threshold) {
	DDAInfo info{ false };

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
	while (!intersectionFound && distance < threshold) {
		if (ray.x < ray.y) {
			currentTile.x += step.x;
			distance = ray.x;
			ray.x += unitStepSize.x;
			info.side = 0;
		} else {
			currentTile.y += step.y;
			distance = ray.y;
			ray.y += unitStepSize.y;
			info.side = 1;
		}

		if (currentTile.x >= 0 && currentTile.x < m_level->getSize().x &&
			currentTile.y >= 0 && currentTile.y < m_level->getSize().y) {
			if (m_level->isTileSolid(currentTile)) intersectionFound = true;
		}
	}

	if (intersectionFound && distance < threshold) {
		info.hit = true;
		info.intersectionPoint = start + dir * distance;
		info.euclideanDist = distance;
		if (info.side) info.perpendicularDist = ray.y -unitStepSize.y;
		else info.perpendicularDist = ray.x -unitStepSize.x;
	}

	return info;
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

	sf::VertexArray viewArea{ sf::TriangleFan };
	viewArea.append({ player.getPosition() * m_tileSize, sf::Color::Yellow });
	size_t numRays = 1200;
	for (size_t i = 0; i < numRays; ++i) {
		float angle = (player.getAngle() - player.getFov() / 2.0f) + (i / static_cast<float>(numRays)) * player.getFov();
		sf::Vector2f rayDir = sf::Vector2f{ std::cosf(angle), -std::sinf(angle) };

		DDAInfo info = dda(player.getPosition(), rayDir, 5.0f);
		if (info.hit) {
			viewArea.append({ info.intersectionPoint * m_tileSize, sf::Color::Yellow });
		} else {
			viewArea.append({ (player.getPosition() + rayDir * 5.0f) * m_tileSize, sf::Color::Yellow });
		}
	}

	m_window->draw(m_levelVertecies);
	m_window->draw(viewArea);
	m_window->draw(playerCircle);
}


RaycastRenderer::RaycastRenderer(sf::RenderWindow* window, Level* level)
	: Renderer{ window, level } {
}


void RaycastRenderer::render(const Player& player) {
	m_screen.clear();

	size_t numRays = m_window->getSize().x;
	for (size_t i = 0; i < numRays; ++i) {
		float angle = (player.getAngle() - player.getFov() / 2.0f) + (i / static_cast<float>(numRays)) * player.getFov();
		sf::Vector2f rayDir = sf::Vector2f{ std::cosf(angle), -std::sinf(angle) };

		DDAInfo info = dda(player.getPosition(), rayDir, 20.0f);
		if (info.hit) {
			float height = static_cast<int>(m_window->getSize().y) / info.perpendicularDist;
			sf::Color color = (info.side) ? sf::Color::White : sf::Color{ 150, 150, 150 };
			m_screen.append({ { static_cast<float>(i), (m_window->getSize().y - height) / 2.0f }, color });
			m_screen.append({ { static_cast<float>(i), (m_window->getSize().y + height) / 2.0f }, color });
		}
	}

	m_window->draw(m_screen);
}

#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window, Level* level)
	: m_window{ window }, m_level{ level } {

}


Renderer::DDAInfo Renderer::dda(const sf::Vector2f& start, const sf::Vector2f& dir, float threshold) {
	DDAInfo info{ false };

	sf::Vector2f unitStepSize = { std::fabs(1.0f / dir.x), std::fabs(1.0f / dir.y) };
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
		if (info.side) info.distance = ray.y - unitStepSize.y;
		else info.distance = ray.x - unitStepSize.x;
	}

	return info;
}


void Renderer::render(const Player& player) {
	m_screen.clear();

	size_t numRays = m_window->getSize().x;
	for (size_t i = 0; i < numRays; ++i) {
		float cameraX = 2.0f * (static_cast<float>(i) / numRays) - 1.0f;
		sf::Vector2f rayDir = player.getDirection() + player.getCameraPlane() * cameraX;

		DDAInfo info = dda(player.getPosition(), rayDir, 20.0f);
		if (info.hit) {
			float height = static_cast<float>(m_window->getSize().y) / (info.distance);
			sf::Color color = (info.side) ? sf::Color::White : sf::Color{ 150, 150, 150 };
			
			m_screen.append({ { static_cast<float>(i), (m_window->getSize().y - height) / 2.0f }, color });
			m_screen.append({ { static_cast<float>(i), (m_window->getSize().y + height) / 2.0f }, color });
		}
	}

	m_window->draw(m_screen);
}

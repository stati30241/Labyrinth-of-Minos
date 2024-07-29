#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window, Level* level)
	: m_window{ window }, m_level{ level } {
	m_texture = TextureManager::get("textureAtlas.png").texture.copyToImage();
	m_floor.create(m_window->getSize().x, m_window->getSize().y);
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
	float centerY = m_window->getSize().y / 2.0f;
	
	for (size_t j = m_window->getSize().y / 2; j < m_window->getSize().y; ++j) {
		sf::Vector2f distance;
		distance.x = centerY / (j - centerY);
		if (distance.x > 5.0f) continue;

		sf::Vector2f start = player.getPosition() + distance.x * (player.getDirection() - player.getCameraPlane());
		sf::Vector2f end = player.getPosition() + distance.x * (player.getDirection() + player.getCameraPlane());

		sf::Vector2f step = (end - start) / static_cast<float>(m_window->getSize().x);

		for (size_t i = 0; i < m_window->getSize().x; ++i) {
			sf::Vector2f cellPos = start;
			start += step;
			sf::Vector2i cell = { static_cast<int>(std::floorf(cellPos.x)), static_cast<int>(std::floorf(cellPos.y)) };
			sf::Vector2i textPos = sf::Vector2i{ 32.0f * (cellPos - cell) };
			
			sf::Uint8 colorValue = static_cast<std::uint8_t>(255.0f * std::powf(1.0f - distance.x / 5.0f, 3));
			sf::Color color = { colorValue, colorValue, colorValue };

			m_floor.setPixel(i, j, m_texture.getPixel(32 + textPos.x, textPos.y) * color);
			m_floor.setPixel(i, m_window->getSize().y - j - 1, m_texture.getPixel(32 + textPos.x, textPos.y) * color);
		}
	}
	sf::Texture t;
	t.loadFromImage(m_floor);
	sf::Sprite s{ t };
	m_window->draw(s);

	size_t numRays = m_window->getSize().x;
	for (size_t i = 0; i < numRays; ++i) {
		float cameraX = 2.0f * (static_cast<float>(i) / numRays) - 1.0f;
		sf::Vector2f rayDir = player.getDirection() + player.getCameraPlane() * cameraX;

		DDAInfo info = dda(player.getPosition(), rayDir, 5.0f);
		if (info.hit) {
			float height = static_cast<float>(m_window->getSize().y) / (info.distance);

			sf::Uint8 colorValue = static_cast<std::uint8_t>(255.0f * std::powf(1.0f - info.distance / 5.0f, 3));
			sf::Color color = { colorValue, colorValue, colorValue };

			float textureIndex = (!info.side) ? static_cast<int>(32.0f * (info.intersectionPoint.y - (int)info.intersectionPoint.y)) :
				static_cast<int>(32.0f * (info.intersectionPoint.x - (int)info.intersectionPoint.x));
			
			m_screen.append({ { static_cast<float>(i), centerY - height / 2.0f }, color, {textureIndex, 0.0f} });
			m_screen.append({ { static_cast<float>(i), centerY + height / 2.0f }, color, {textureIndex, 32.0f} });
		}
	}

	sf::RenderStates states{ &TextureManager::get("textureAtlas.png").texture };
	m_window->draw(m_screen, states);
}

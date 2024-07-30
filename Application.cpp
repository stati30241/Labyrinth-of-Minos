#include "Application.h"


Application::Application() {
	initialize();
}



Application::~Application() {
	delete m_window;
	delete m_renderer;
}


void Application::initialize() {
	sf::VideoMode windowSize = { 1200u, 676u };
	sf::String windowTitle = "Labyrinth of Minos";
	sf::Uint32 windowStyle = sf::Style::Close;
	m_window = new sf::RenderWindow{ windowSize, windowTitle, windowStyle };
	m_window->setMouseCursorVisible(false);

	m_level = Level{ { 16, 9 } };
	m_player = Player{ { 8.5f, 4.5f }, {std::numbers::pi / 2.0f, 0.0f }, std::numbers::pi / 2.0f};
	m_renderer = new Renderer{ m_window, &m_level };
}


void Application::updatePlayerPosition(float deltaTime) {
	const sf::Vector2f newPlayerPos = m_player.getPosition() + m_player.getVelocity() * deltaTime;
	if (newPlayerPos.x < 0.0f || newPlayerPos.x >= m_level.getSize().x ||
		newPlayerPos.y < 0.0f || newPlayerPos.y >= m_level.getSize().y) return;

	const sf::Vector2i currentPlayerTile = sf::Vector2i{ m_player.getPosition() };
	const sf::Vector2i newPlayerTile = sf::Vector2i{ newPlayerPos };
	if (!m_level.isTileSolid(newPlayerTile)) m_player.setPosition(newPlayerPos);
	else if (!m_level.isTileSolid({ newPlayerTile.x, currentPlayerTile.y })) 
		m_player.setPosition({ newPlayerPos.x, m_player.getPosition().y });
	else if (!m_level.isTileSolid({ currentPlayerTile.x, newPlayerTile.y }))
		m_player.setPosition({ m_player.getPosition().x, newPlayerPos.y });
}


void Application::handleInputs() {
	sf::Event sfmlEvent{ };

	float maxSpeed = 1.0f;

	while (m_window->pollEvent(sfmlEvent)) {

		switch (sfmlEvent.type) {
		case sf::Event::Closed:
			m_window->close();
			break;

		case sf::Event::KeyPressed:
			if (sfmlEvent.key.code == sf::Keyboard::W) {
				m_player.setVelocity({ std::cosf(m_player.getAngle().x), -std::sinf(m_player.getAngle().x) });
			} if (sfmlEvent.key.code == sf::Keyboard::A) {
				m_player.setVelocity({ std::sinf(m_player.getAngle().x), std::cos(m_player.getAngle().x) });
			} if (sfmlEvent.key.code == sf::Keyboard::S) {
				m_player.setVelocity({ -std::cosf(m_player.getAngle().x), std::sinf(m_player.getAngle().x) });
			} if (sfmlEvent.key.code == sf::Keyboard::D) {
				m_player.setVelocity({ -std::sinf(m_player.getAngle().x), -std::cosf(m_player.getAngle().x) });
			} break;

		case sf::Event::KeyReleased:
			if (sfmlEvent.key.code == sf::Keyboard::W || sfmlEvent.key.code == sf::Keyboard::S ||
				sfmlEvent.key.code == sf::Keyboard::A || sfmlEvent.key.code == sf::Keyboard::D) {
				m_player.setVelocity({ 0.0f, 0.0f });
			} break;
		}
	}
}


void Application::update() {
	float deltaTime = m_clock.restart().asSeconds();

	sf::Vector2f mouseOffset = sf::Mouse::getPosition(*m_window) - sf::Vector2f{ m_window->getSize() } / 2.0f;
	m_player.setAngle(mouseOffset * deltaTime + m_player.getAngle());
	
	// Clamp vertical angle
	float maxAngle = std::numbers::pi / 4.0f;
	if (m_player.getAngle().y < -maxAngle) m_player.setAngle({ m_player.getAngle().x, -maxAngle });
	if (m_player.getAngle().y > maxAngle) m_player.setAngle({ m_player.getAngle().x, maxAngle });

	updatePlayerPosition(deltaTime);

	sf::Mouse::setPosition(sf::Vector2i{ m_window->getSize() / 2u }, *m_window);

	m_window->setTitle("FPS: " + std::to_string(1.0f / deltaTime));
}


void Application::render() {
	m_window->clear();

	m_renderer->render(m_player);

	m_window->display();
}


void Application::run() {
	while (m_window->isOpen()) {
		handleInputs();
		update();
		render();
	}
}

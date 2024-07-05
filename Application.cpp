#include "Application.h"


Application::Application() {
	initialize();
}



Application::~Application() {
	delete m_window;
	delete m_renderer;
}


void Application::initialize() {
	sf::VideoMode windowSize = { 1200u, 675u };
	sf::String windowTitle = "Labyrinth of Minos";
	sf::Uint32 windowStyle = sf::Style::Close;
	m_window = new sf::RenderWindow{ windowSize, windowTitle, windowStyle };

	m_level = Level{ { 5, 5 } };
	m_renderer = new MiniMapRenderer(m_window, &m_level);
}


void Application::handleInputs() {
	sf::Event sfmlEvent{ };
	while (m_window->pollEvent(sfmlEvent)) {

		switch (sfmlEvent.type) {
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}
}


void Application::update() {
	float deltaTime = m_clock.restart().asSeconds();
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

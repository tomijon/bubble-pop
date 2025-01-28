#include "menu.hpp"



MainMenu::MainMenu(sf::RenderWindow& render) : Menu(render) {
	sf::Vector2u windowDimensions = render.getSize();

	play = new sf::Text(font, "Play");
	leave = new sf::Text(font, "Leave");

	play->setCharacterSize(180);
	leave->setCharacterSize(100);

	play->setPosition({
		(windowDimensions.x / 2.0f) - (play->getLocalBounds().size.x / 2.0f),
		(windowDimensions.y / 2.0f) - (play->getLocalBounds().size.y)
		});
	leave->setPosition({
		(windowDimensions.x / 2.0f) - (leave->getLocalBounds().size.x / 2.0f),
		(windowDimensions.y / 2.0f) + (leave->getLocalBounds().size.y)
		});

	play->setFillColor(sf::Color(0, 255, 0, 255));
	leave->setFillColor(sf::Color(255, 0, 0, 255));
}


MainMenu::~MainMenu() {
	delete play;
	delete leave;
}


void MainMenu::Draw() {
	render.draw(*play);
	render.draw(*leave);
}


ButtonTypes::ButtonType MainMenu::CheckButtonPressed(sf::Vector2i mousePos) {
	// Check play button.
	sf::Vector2<float> converted(mousePos.x, mousePos.y);
	if (play->getGlobalBounds().contains(converted)) return ButtonTypes::ButtonType::PLAY;
	if (leave->getGlobalBounds().contains(converted)) return ButtonTypes::ButtonType::QUIT;

	return ButtonTypes::ButtonType::NONE;
}

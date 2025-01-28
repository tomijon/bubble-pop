#pragma once

#include <string>

#include <SFML/Graphics.hpp>


const sf::Font font("resources/Splashextrabold-5y4v.ttf");

/**
 * @brief Namespace to localise enum types.
 */
namespace ButtonTypes {

	/**
	 * Available button types for all menus.
	 */
	enum ButtonType {
		PLAY,
		QUIT,
		NONE
	};
}


/**
 * @brief Base menu class.
 * 
 * All other menus can inherit from this class and implement the virtual functions
 */
class Menu {
public:
	/**
	 * @brief Menu Constructor.
	 * 
	 * @param render The window to draw to.
	 */
	Menu(sf::RenderWindow& render) : render(render) {}

	/**
	 * @brief Virtual deconstructor.
	 * 
	 * Override if allocating memory.
	 */
	virtual ~Menu() {};

	/**
	 * @brief Handles drawing of menu buttons.
	 * 
	 * Must be overriden to draw buttons. 
	 */
	virtual void Draw() = 0;

	/**
	 * @brief Handles checking for button presses.
	 * @param mousePos The position of the mouse on the screen.
	 * @return The type of the button that was pressed.
	 */
	virtual ButtonTypes::ButtonType CheckButtonPressed(sf::Vector2i mousePos) = 0;

protected:
	sf::RenderWindow& render;
};


/**
 * @brief Main menu to show on application startup.
 */
class MainMenu : public Menu {
public:
	/**
	 * @brief Main menu constructor.
	 * @param render The window the draw the buttons to.
	 * 
	 * Handles the creation of the play and quit buttons.
	 */
	MainMenu(sf::RenderWindow& render);

	/**
	 * @brief Deconstructs the menu.
	 * 
	 * Deletes all buttons.
	 */
	~MainMenu(void);

	/**
	 * @brief Draw the play and quit buttons.
	 */
	void Draw() override;

	/**
	 * @brief Check if any of the buttons have been pressed.
	 * 
	 * @param mousePos The position of the mouse on the screen.
	 * @return The type of button that was pressed.
	 */
	ButtonTypes::ButtonType CheckButtonPressed(sf::Vector2i mousePos) override;

private:
	sf::Text* play;
	sf::Text* leave;
};

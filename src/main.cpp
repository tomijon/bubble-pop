#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "bubble.hpp"
#include "menu.hpp"


enum GameState {
    MENU,
    PAUSED,
    PLAYING,
    SCORE
};


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Bubble Trouble", sf::Style::None);
    window.setFramerateLimit(165);

    Bubble::SetMaxRadius(0.1 * window.getSize().y);
    Bubble::SetMinRadius(0.05 * window.getSize().x);

    BubbleGameSettings settings = {
        2.0f, 1.0f, 100
    };
    BubbleGame game(window, settings);

    MainMenu mainMenu(window);
	GameState currentState = MENU;

    sf::Text score(font, "Score: 0");
	score.setCharacterSize(200);

    auto last = std::chrono::high_resolution_clock::now();
    auto now = last;
    float dt;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const auto& pressedEvent = event->getIf<sf::Event::KeyPressed>();
                if (pressedEvent->code == sf::Keyboard::Key::Escape) {
                    exit(0);
                }

                if (currentState == PLAYING) {
                    game.AttemptBubblePop(pressedEvent->code);
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto& clickEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (clickEvent->button == sf::Mouse::Button::Left) {
                    if (currentState == MENU) {
                        ButtonTypes::ButtonType result = mainMenu.CheckButtonPressed(clickEvent->position);
                        if (result == ButtonTypes::ButtonType::PLAY) {
                            currentState = PLAYING;
                            last = std::chrono::high_resolution_clock::now();
                        }
                        else if (result == ButtonTypes::ButtonType::QUIT) {
                            exit(0);
                        }
                    }
                }
            }
        }
        
        window.clear();

        if (currentState == MENU) mainMenu.Draw();
        else if (currentState == PLAYING) {
            now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> duration = now - last;
            dt = duration.count();
            
            game.Update(dt);
            last = now;

            if (game.GetGameState() == BubbleGameState::GAMEOVER) {
                currentState = SCORE;
                score.setString("Score: " + std::to_string(game.GetScore()));
                continue;
            }
            game.Draw();
        }
        else if (currentState == SCORE) {
            score.setPosition(
                sf::Vector2f(
                    (window.getSize().x / 2.0f) - (score.getLocalBounds().size.x / 2.0f),
                    (window.getSize().y / 2.0f) - (score.getLocalBounds().size.y / 2.0f)
                ));
            window.draw(score);
        }

        window.display();
    }
}

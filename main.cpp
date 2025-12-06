#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "GameState.h"

enum class GameStateEnum
{
    Menu,
    Playing,
    Scores,
    Exiting
};

int main()
{
    const int SZEROKOSC = 800;
    const int WYSOKOSC = 600;

    sf::RenderWindow window(
        sf::VideoMode(SZEROKOSC, WYSOKOSC),
        "Arkanoid"
    );
    window.setFramerateLimit(60);

    Menu menu(window.getSize().x, window.getSize().y);
    Game game;
    GameState snapshot;

    GameStateEnum currentState = GameStateEnum::Menu;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                // MENU 
                if (currentState == GameStateEnum::Menu)
                {
                    if (event.key.code == sf::Keyboard::Up)
                        menu.moveUp();
                    else if (event.key.code == sf::Keyboard::Down)
                        menu.moveDown();
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        int selected = menu.getSelectedItemIndex();

                        if (selected == 0)      // Nowa gra
                        {
                            currentState = GameStateEnum::Playing;
                        }
                        else if (selected == 1) // Wczytaj gre
                        {
                            GameState loaded;
                            if (loaded.loadFromFile("zapis.txt"))
                            {
                                loaded.apply(
                                    game.getPaddle(),
                                    game.getBall(),
                                    game.getBricks()
                                );
                                std::cout << "Gra wczytana!\n";
                                currentState = GameStateEnum::Playing;
                            }
                            else
                            {
                                std::cout << "Brak pliku zapisu lub blad odczytu.\n";
                            }
                        }
                        else if (selected == 2) // Ostatnie wyniki
                        {
                            currentState = GameStateEnum::Scores;
                        }
                        else if (selected == 3) // Wyjscie
                        {
                            currentState = GameStateEnum::Exiting;
                            window.close();
                        }
                    }
                }
                // PLAYING
                else if (currentState == GameStateEnum::Playing)
                {
                    // Zapis gry na F5
                    if (event.key.code == sf::Keyboard::F5)
                    {
                        snapshot.capture(
                            game.getPaddle(),
                            game.getBall(),
                            game.getBlocks()
                        );

                        if (snapshot.saveToFile("zapis.txt"))
                            std::cout << "Gra zapisana!\n";
                        else
                            std::cout << "Blad zapisu!\n";
                    }

                    if (event.key.code == sf::Keyboard::Escape)
                        currentState = GameStateEnum::Menu;
                }
                // SCORES
                else if (currentState == GameStateEnum::Scores)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        currentState = GameStateEnum::Menu;
                }
            }
        }

        // UPDATE
        if (currentState == GameStateEnum::Playing)
            game.update(dt);

        // RENDER
        window.clear(sf::Color(40, 30, 20));

        if (currentState == GameStateEnum::Menu)
        {
            menu.draw(window);
        }
        else if (currentState == GameStateEnum::Playing)
        {
            game.render(window);
        }
        else if (currentState == GameStateEnum::Scores)
        {
            sf::Font font;
            font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

            sf::Text txt;
            txt.setFont(font);
            txt.setString("Tu beda ostatnie wyniki.\nESC - powrot do menu");
            txt.setCharacterSize(24);
            txt.setFillColor(sf::Color::White);
            txt.setPosition(40.f, 40.f);

            window.draw(txt);
        }

        window.display();
    }

    return 0;
}

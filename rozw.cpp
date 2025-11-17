#include <SFML/Graphics.hpp>

#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Bricks.h"

int main()
{
    const int SZEROKOSC = 800;
    const int WYSOKOSC = 600;

    sf::RenderWindow window(
        sf::VideoMode(SZEROKOSC, WYSOKOSC),
        "GraKP"
    );
    window.setFramerateLimit(60);

    sf::Clock deltaClock; // do liczenia dt

    Paddle paletka(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 30.f),
        sf::Vector2f(200.f, 20.f),
        sf::Vector2f(400.f, 0.f)
    );

    Ball pilka(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC / 2.f),
        20.f,
        sf::Vector2f(-300.f, -300.f)
    );

    // zarz¹dzanie bloczkami
    Bricks bricks;
    const int ILOSC_KOLUMN = 12;
    const int ILOSC_WIERSZY = 4;
    bricks.initGrid(ILOSC_KOLUMN, ILOSC_WIERSZY, static_cast<float>(SZEROKOSC));

    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // logika gry 
        paletka.ruch(
            dt,
            sf::Vector2f(
                static_cast<float>(SZEROKOSC),
                static_cast<float>(WYSOKOSC)
            )
        );

        pilka.ruch(
            dt,
            sf::Vector2f(
                static_cast<float>(SZEROKOSC),
                static_cast<float>(WYSOKOSC)
            ),
            paletka
        );

        bricks.update(pilka);

        //rysowanie 
        window.clear(sf::Color(40, 30, 20));
        paletka.draw(window);
        pilka.draw(window);
        bricks.draw(window);
        window.display();
    }

    return 0;
}

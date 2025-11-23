#include "Game.h"

Game::Game()
    : m_window(sf::VideoMode(SZEROKOSC, WYSOKOSC), "Arcystonid"),
    m_paletka(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 30.f),
        sf::Vector2f(200.f, 20.f),
        sf::Vector2f(400.f, 0.f)
    ),
    m_pilka(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC / 2.f),
        20.f,
        sf::Vector2f(-300.f, -300.f)
    )
{
    m_window.setFramerateLimit(60);

    const int ILOSC_KOLUMN = 12;
    const int ILOSC_WIERSZY = 4;
    m_bricks.initGrid(ILOSC_KOLUMN, ILOSC_WIERSZY, static_cast<float>(SZEROKOSC));
}

void Game::run()
{
    while (m_window.isOpen())
    {
        sf::Time dt = m_deltaClock.restart();

        processEvents();
        update(dt);
        render();
    }
}


void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}


void Game::update(sf::Time dt)
{
    m_paletka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        )
    );

    m_pilka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        ),
        m_paletka
    );

    m_bricks.update(m_pilka);
}

void Game::render()
{
    m_window.clear(sf::Color(40, 30, 20));

    m_paletka.draw(m_window);
    m_pilka.draw(m_window);
    m_bricks.draw(m_window);

    m_window.display();
}

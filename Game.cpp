#include "Game.h"

Game::Game()
    : m_paletka(
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
    const int ILOSC_KOLUMN = 12;
    const int ILOSC_WIERSZY = 4;
    m_bricks.initGrid(ILOSC_KOLUMN, ILOSC_WIERSZY, static_cast<float>(SZEROKOSC));
}

void Game::update(sf::Time dt)
{
    // ruch paletki
    m_paletka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        )
    );

    // ruch pilki + kolizja z paletka i scianami
    m_pilka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        ),
        m_paletka
    );

    // kolizje pilki z bloczkami + usuwanie zniszczonych
    m_bricks.update(m_pilka);
}

void Game::render(sf::RenderTarget& target)
{
    m_paletka.draw(target);
    m_pilka.draw(target);
    m_bricks.draw(target);
}

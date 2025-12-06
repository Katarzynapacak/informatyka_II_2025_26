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
    // konfiguracja gwiazdy jest w konstruktorze Star
    reset();
}

void Game::reset()
{
    const int ILOSC_KOLUMN = 12;
    const int ILOSC_WIERSZY = 4;

    m_paletka.setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 30.f));

    m_pilka.setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC / 2.f));
    m_pilka.setVelocity(sf::Vector2f(-300.f, -300.f));

    m_bricks.initGrid(ILOSC_KOLUMN, ILOSC_WIERSZY, static_cast<float>(SZEROKOSC));
    m_bricks.resetFirstCollision();

    // gwiazda na srodku ekranu, widoczna na starcie
    m_star.setPosition(
        sf::Vector2f(
            static_cast<float>(SZEROKOSC) / 2.f,
            static_cast<float>(WYSOKOSC) / 2.f
        )
    );
    m_star.show();
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

    // kolizje pilki z bloczkami
    m_bricks.update(m_pilka, dt);

    // po pierwszej kolizji chowamy gwiazde
    if (m_bricks.hasFirstCollision())
        m_star.hide();
}

bool Game::isBallOutOfBounds(float windowHeight) const
{
    const sf::FloatRect bounds = m_pilka.getGlobalBounds();
    return bounds.top > windowHeight;
}

void Game::render(sf::RenderTarget& target)
{
    m_paletka.draw(target);
    m_pilka.draw(target);
    m_bricks.draw(target);

    // gwiazda jako osobny obiekt
    m_star.draw(target);
}

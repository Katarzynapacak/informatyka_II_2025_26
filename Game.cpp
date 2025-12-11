#include "Game.h"
#include <sstream> 

Game::Game()
    : m_paletka( //pozycja rozmiar i predkosc paletki
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 30.f), 
        sf::Vector2f(200.f, 20.f),                      
        sf::Vector2f(400.f, 0.f)                        
    ),
    m_pilka( //pozycja rozmiar i predkosc pilki
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC / 2.f),  
        20.f,                                           
        sf::Vector2f(-300.f, -300.f)                    
    )
{

    if (!m_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
    {
    }

    m_brickCounterText.setFont(m_font);
    m_brickCounterText.setCharacterSize(20);
    m_brickCounterText.setFillColor(sf::Color::White);
    m_brickCounterText.setPosition(10.f, 10.f); 

    reset();
}

// poczatkowy stan gry
void Game::reset()
{
    const int ILOSC_KOLUMN = 12;
    const int ILOSC_WIERSZY = 4;

    // Startowa pozycja paletki
    m_paletka.setPosition(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 30.f)
    );

    // Startowa pozycja i prêdkoœæ pi³ki
    m_pilka.setPosition(
        sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC / 2.f)
    );
    m_pilka.setVelocity(
        sf::Vector2f(-300.f, -300.f)
    );

    //siatka klocków
    m_bricks.initGrid(
        ILOSC_KOLUMN,
        ILOSC_WIERSZY,
        static_cast<float>(SZEROKOSC)
    );

    // Gwiazda na pocz¹tku gry pojawia siê i bedzie spadac
    m_star.setPosition(
        sf::Vector2f(
            static_cast<float>(SZEROKOSC) / 2.f,
            40.f 
        )
    );
    m_star.show();
}

void Game::update(sf::Time dt)
{
    // Ruch paletki
    m_paletka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        )
    );

    // Ruch pi³ki, odbicia od œcian i paletki
    m_pilka.ruch(
        dt,
        sf::Vector2f(
            static_cast<float>(SZEROKOSC),
            static_cast<float>(WYSOKOSC)
        ),
        m_paletka
    );

    // Kolizje pi³ki z klockami, usuwanie zbitych
    m_bricks.update(m_pilka, dt);
    m_star.update(dt, static_cast<float>(WYSOKOSC));

    // Aktualizacja napisu z licznikiem zbitych klocków
    int destroyed = getDestroyedBricks();
    std::ostringstream oss;
    oss << "Zbite klocki: " << destroyed;
    m_brickCounterText.setString(oss.str());
}

// Sprawdzenie czy pi³ka wypad³a poza dó³ ekranu 
bool Game::isBallOutOfBounds(float windowHeight) const
{
    const sf::FloatRect bounds = m_pilka.getGlobalBounds();
    return bounds.top > windowHeight;
}

// Rysowanie 
void Game::render(sf::RenderTarget& target)
{
    m_paletka.draw(target);
    m_pilka.draw(target);
    m_bricks.draw(target);
    m_star.draw(target);
    target.draw(m_brickCounterText);
}

#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
private:
    sf::RectangleShape m_shape;      // kszta³t paletki
    sf::Vector2f velocity{ 200.f, 0.f };

public:
    Paddle(sf::Vector2f startPos, sf::Vector2f rozmiar, sf::Vector2f startVel);
    void draw(sf::RenderTarget& window);
    void ruch(sf::Time dt, sf::Vector2f windowWidth);
    sf::FloatRect getGlobalBounds() { return m_shape.getGlobalBounds(); }
};

// -------- IMPLEMENTACJA --------

Paddle::Paddle(sf::Vector2f startPos, sf::Vector2f rozmiar, sf::Vector2f startVel)
{
    velocity = startVel;
    m_shape.setPosition(startPos);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setSize(rozmiar);
    m_shape.setOrigin(rozmiar.x / 2.f, rozmiar.y / 2.f);
}

void Paddle::draw(sf::RenderTarget& window)
{
    window.draw(m_shape);
}

void Paddle::ruch(sf::Time dt, sf::Vector2f windowWH)
{
    // obs³uga klawiszy — ruch w lewo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_shape.move({ -velocity.x * dt.asSeconds(), 0.f });
    }

    // obs³uga klawiszy — ruch w prawo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_shape.move({ velocity.x * dt.asSeconds(), 0.f });
    }

    // --- kolizja ze œcianami ---
    sf::Vector2f pos = m_shape.getPosition();
    sf::Vector2f size = m_shape.getSize();

    float halfW = size.x / 2.f;

    // lewa œciana
    if (pos.x - halfW < 0.f)
        m_shape.setPosition(halfW, pos.y);

    // prawa œciana
    if (pos.x + halfW > windowWH.x)
        m_shape.setPosition(windowWH.x - halfW, pos.y);

    
}



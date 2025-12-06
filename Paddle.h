#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
private:
    sf::RectangleShape m_shape;
    sf::Vector2f       velocity{ 200.f, 0.f };

public:
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    void setPosition(const sf::Vector2f& pos) { m_shape.setPosition(pos); }

    Paddle(sf::Vector2f startPos, sf::Vector2f rozmiar, sf::Vector2f startVel)
    {
        velocity = startVel;
        m_shape.setPosition(startPos);
        m_shape.setSize(rozmiar);
        m_shape.setOrigin(rozmiar.x / 2.f, rozmiar.y / 2.f);
        m_shape.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderTarget& window)
    {
        window.draw(m_shape);
    }
    
    void ruch(sf::Time dt, sf::Vector2f windowWH)// ruch 
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            m_shape.move(-velocity.x * dt.asSeconds(), 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            m_shape.move(velocity.x * dt.asSeconds(), 0.f);
        }

        sf::Vector2f pos = m_shape.getPosition();
        sf::Vector2f size = m_shape.getSize();
        float halfW = size.x / 2.f;

        if (pos.x - halfW < 0.f)
            m_shape.setPosition(halfW, pos.y);

        if (pos.x + halfW > windowWH.x)
            m_shape.setPosition(windowWH.x - halfW, pos.y);
    }

    sf::FloatRect getGlobalBounds() const
    {
        return m_shape.getGlobalBounds();
    }
};

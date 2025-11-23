#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball
{
private:
    sf::CircleShape m_shape;
    sf::Vector2f    velocity{ 200.f, 200.f };

public:
    Ball(sf::Vector2f startPos, float radius, sf::Vector2f startVel)
    {
        velocity = startVel;
        m_shape.setPosition(startPos);
        m_shape.setRadius(radius);
        m_shape.setOrigin(radius, radius);
        m_shape.setFillColor(sf::Color::Green);
    }

    void draw(sf::RenderTarget& window)
    {
        window.draw(m_shape);
    }

    void odbijY()
    {
        velocity.y = -velocity.y;
        m_shape.move(0.f, velocity.y * 0.02f);
    }

    void ruch(sf::Time dt, sf::Vector2f windowWH, Paddle& pd1)
    {
        // ruch pi³ki
        m_shape.move(velocity * dt.asSeconds());

        float xb = m_shape.getPosition().x;
        float yb = m_shape.getPosition().y;
        float rb = m_shape.getRadius();

        // kolizja z lew¹/praw¹ œcian¹
        if (xb - rb <= 0.f || xb + rb >= windowWH.x)
        {
            velocity.x = -velocity.x;
        }

        // kolizja z górn¹ œcian¹
        if (yb - rb <= 0.f)
        {
            velocity.y = -velocity.y;
        }

        // dó³ ekranu – zatrzymanie pi³ki
        if (yb + rb >= windowWH.y)
        {
            velocity.x = 0.f;
            velocity.y = 0.f;
        }

        // kolizja z paletk¹
        if (getGlobalBounds().intersects(pd1.getGlobalBounds()))
        {
            velocity.y = -velocity.y;
            m_shape.move(0.f, velocity.y * 0.02f);
        }
    }

    sf::FloatRect getGlobalBounds() const
    {
        return m_shape.getGlobalBounds();
    }
};

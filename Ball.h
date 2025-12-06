#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball
{
private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_velocity;

public:
    Ball(const sf::Vector2f& startPos, float radius, const sf::Vector2f& startVel)
    {
        m_shape.setRadius(radius);
        m_shape.setOrigin(radius, radius);
        m_shape.setPosition(startPos);
        m_velocity = startVel;
        m_shape.setFillColor(sf::Color::White);
    }

    // move ball
    void ruch(sf::Time dt, const sf::Vector2f& windowWH, Paddle& pad)
    {
        m_shape.move(m_velocity * dt.asSeconds());

        float x = m_shape.getPosition().x;
        float y = m_shape.getPosition().y;
        float r = m_shape.getRadius();

        // walls
        if (x - r < 0.f || x + r > windowWH.x)
            m_velocity.x = -m_velocity.x;

        if (y - r < 0.f)
            m_velocity.y = -m_velocity.y;

        // paddle
        if (m_shape.getGlobalBounds().intersects(pad.getGlobalBounds()))
        {
            m_velocity.y = -m_velocity.y;
        }
    }

    void draw(sf::RenderTarget& window)
    {
        window.draw(m_shape);
    }

    void odbijY()
    {
        m_velocity.y = -m_velocity.y;
    }

    // getters
    sf::Vector2f  getPosition()     const { return m_shape.getPosition(); }
    sf::Vector2f  getVelocity()     const { return m_velocity; }
    sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }

    // setters
    void setPosition(const sf::Vector2f& pos) { m_shape.setPosition(pos); }
    void setVelocity(const sf::Vector2f& vel) { m_velocity = vel; }
};

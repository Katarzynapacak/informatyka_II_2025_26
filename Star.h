#pragma once
#include <SFML/Graphics.hpp>

class Star
{
private:
    sf::CircleShape m_shape;
    bool m_visible;

public:
    Star()
        : m_shape(), m_visible(false)
    {
        m_shape.setRadius(15.f);
        m_shape.setOrigin(15.f, 15.f);
        m_shape.setFillColor(sf::Color::Yellow); 
    }

    void setPosition(const sf::Vector2f& pos)
    {
        m_shape.setPosition(pos);
    }

    void show()
    {
        m_visible = true;
    }

    void hide()
    {
        m_visible = false;
    }

    bool isVisible() const
    {
        return m_visible;
    }

    void draw(sf::RenderTarget& target) const
    {
        if (m_visible)
            target.draw(m_shape);
    }
};

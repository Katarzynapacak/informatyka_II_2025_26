#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

//spadaj¹ca gwiazdê na pocz¹tku gry
class Star
{
private:
    sf::ConvexShape m_shape;   // kszta³t gwiazdy
    bool m_visible;            
    sf::Vector2f m_velocity;   // prêdkoœæ opadania gwiazdy

    // funkcja buduj¹ca 5-ramienn¹ gwiazdê
    void buildStar(float outerRadius, float innerRadius)
    {
        const int pointCount = 10;           
        m_shape.setPointCount(pointCount);

        // K¹t miêdzy kolejnymi punktami 
        const float step = 36.f;
        float angleDeg = -90.f;
        const float pi = 3.14159265359f;

        for (int i = 0; i < pointCount; ++i)
        {
            float radius = (i % 2 == 0) ? outerRadius : innerRadius;
            float angleRad = angleDeg * pi / 180.f;

            float x = std::cos(angleRad) * radius;
            float y = std::sin(angleRad) * radius;

            m_shape.setPoint(i, sf::Vector2f(x, y));

            angleDeg += step;
        }

        m_shape.setOrigin(0.f, 0.f);
    }

public:
    Star()
        : m_shape(), m_visible(false), m_velocity(0.f, 60.f) 
    {
        //promien zewnêtrzny 20 i wewnêtrzny 8
        buildStar(20.f, 8.f);
        m_shape.setFillColor(sf::Color::Yellow);
    }

    // ustawienie pozycji œrodka gwiazdy na ekranie.
    void setPosition(const sf::Vector2f& pos)
    {
        m_shape.setPosition(pos);
    }

    // widoczna
    void show()
    {
        m_visible = true;
    }

    // niewidoczna
    void hide()
    {
        m_visible = false;
    }

    bool isVisible() const
    {
        return m_visible;
    }

    // spada i jak spadnie to znika
    void update(sf::Time dt, float windowHeight)
    {
        if (!m_visible)
            return;

        m_shape.move(m_velocity * dt.asSeconds());

        sf::FloatRect bounds = m_shape.getGlobalBounds();
        if (bounds.top > windowHeight)
        {
            //poza ekranem
            m_visible = false;
        }
    }

    // rysowanie gwiazdy gdy jest widoczna
    void draw(sf::RenderTarget& target) const
    {
        if (m_visible)
            target.draw(m_shape);
    }
};

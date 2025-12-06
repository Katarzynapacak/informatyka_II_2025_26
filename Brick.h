#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Brick
{
private:
    sf::RectangleShape m_shape;
    int  m_zycie;
    bool m_zniszczony;

    static const std::array<sf::Color, 4>& colorLUT()
    {
        static const std::array<sf::Color, 4> lut = {
            sf::Color::Transparent,
            sf::Color::Yellow,
            sf::Color::Magenta,
            sf::Color::Red
        };
        return lut;
    }

    void aktualizujKolor()
    {
        int idx = m_zycie;
        if (idx < 0) idx = 0;
        if (idx > 3) idx = 3;

        m_shape.setFillColor(colorLUT()[idx]);
    }

public:
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::Vector2f getSize()     const { return m_shape.getSize(); }
    int          getHP()       const { return m_zycie; }

    Brick(sf::Vector2f startPo, sf::Vector2f rozmiar, int L)
        : m_shape(rozmiar),
        m_zycie(L),
        m_zniszczony(false)
    {
        m_shape.setPosition(startPo);
        m_shape.setOrigin(rozmiar.x / 2.f, rozmiar.y / 2.f);
        m_shape.setOutlineColor(sf::Color::White);
        m_shape.setOutlineThickness(1.f);
        aktualizujKolor();
    }

    void draw(sf::RenderTarget& window)
    {
        window.draw(m_shape);
    }

    sf::FloatRect getGlobalBounds() const
    {
        return m_shape.getGlobalBounds();
    }

    bool czyZniszczony() const
    {
        return m_zniszczony;
    }

    void trafienie()
    {
        if (m_zniszczony)
            return;

        if (m_zycie > 0)
            m_zycie--;

        if (m_zycie <= 0)
        {
            m_zycie = 0;
            m_zniszczony = true;
        }

        aktualizujKolor();
    }
};

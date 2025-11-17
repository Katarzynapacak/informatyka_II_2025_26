#pragma once
#include <SFML/Graphics.hpp>
#include <array>   // prosta tablica sta³ego rozmiaru

class Brick : public sf::RectangleShape {
private:
    int punktyZycia;       // 0–3
    bool jestZniszczony;   // jeœli zniszczony to = true

    // tablica kolorów zale¿nych od ¿ycia
    static const std::array<sf::Color, 4> colorLUT;

public:
    Brick(sf::Vector2f startPo, sf::Vector2f rozmiar, int L);

    void aktualizujKolor();              // zmienia kolor klocka w zale¿noœci od punktyZycia
    void trafienie();                    // zmniejsza ¿ycie i ustawia kolor
    void draw(sf::RenderTarget& window); // rysuje klocek
    bool czyZniszczony() { return jestZniszczony; }
};

Brick::Brick(sf::Vector2f startPo, sf::Vector2f rozmiar, int L)
{
    punktyZycia = L;         // liczba ¿yæ
    jestZniszczony = false;  

    this->setPosition(startPo);
    this->setSize(rozmiar);

    this->setFillColor(sf::Color::Yellow);
    this->setOutlineColor(sf::Color::White);
    this->setOutlineThickness(1.f);

    aktualizujKolor();
}

const std::array<sf::Color, 4> Brick::colorLUT = {
    sf::Color::Transparent,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Red
};

void Brick::trafienie()
{
    if (jestZniszczony == true)
        return; 

    punktyZycia--;
    aktualizujKolor();

    if (punktyZycia <= 0)
        jestZniszczony = true;
}

void Brick::aktualizujKolor()
{
    if (punktyZycia >= 0 && punktyZycia <= 3)
        this->setFillColor(colorLUT[punktyZycia]);
}

void Brick::draw(sf::RenderTarget& window)
{
    window.draw(*this);
}



#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Brick.h"
#include "Ball.h"

class Bricks
{
private:
    std::vector<Brick> bloki;
    float rozmiarX{};
    float rozmiarY{};
    int   m_poczatkowaLiczba{};
    bool  m_firstCollision = false; 

public:
    const std::vector<Brick>& getVector() const { return bloki; }
    std::vector<Brick>& getVectorRef() { return bloki; }

    int getDestroyedCount() const
    {
        return m_poczatkowaLiczba - static_cast<int>(bloki.size());
    }

    int  getInitialCount()   const { return m_poczatkowaLiczba; }
    bool hasFirstCollision() const { return m_firstCollision; }
    void resetFirstCollision() { m_firstCollision = false; }

    Bricks() = default;

    // bloczki
    void initGrid(int kolumny, int wiersze, float szerokoscOkna)
    {
        bloki.clear();
        m_firstCollision = false;

        float odstep = 2.f;
        rozmiarX = (szerokoscOkna - (kolumny - 1) * odstep) / kolumny;
        rozmiarY = 20.f;

        m_poczatkowaLiczba = kolumny * wiersze;

        for (int y = 0; y < wiersze; ++y)
        {
            for (int x = 0; x < kolumny; ++x)
            {
                float posX = x * (rozmiarX + odstep);
                float posY = y * (rozmiarY + odstep) + 60.f;

                int zycie = 1;
                if (y == 0) zycie = 1;
                if (y >= 1) zycie = 2;

                bloki.emplace_back(
                    sf::Vector2f(posX, posY),
                    sf::Vector2f(rozmiarX, rozmiarY),
                    zycie
                );
            }
        }
    }

    // kolizja i usuwanie
    void update(Ball& pilka, sf::Time dt)
    {
        (void)dt;

        for (auto& blk : bloki)
        {
            if (!blk.czyZniszczony() &&
                pilka.getGlobalBounds().intersects(blk.getGlobalBounds()))
            {
                blk.trafienie();
                pilka.odbijY();

                if (!m_firstCollision)
                    m_firstCollision = true;
            }
        }

        for (int i = static_cast<int>(bloki.size()) - 1; i >= 0; --i)
        {
            if (bloki[i].czyZniszczony())
                bloki.erase(bloki.begin() + i);
        }
    }

    void draw(sf::RenderTarget& window)
    {
        for (auto& blk : bloki)
            blk.draw(window);
    }
};

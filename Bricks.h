#pragma once
#include <vector>
#include "Brick.h"
#include "Ball.h"

class Bricks : public Brick    
{
private:
    std::vector<Brick> bloki;  // tablica cegie³
    float rozmiarX;
    float rozmiarY;

public:

    Bricks()
        : Brick({ 0.f,0.f }, { 0.f,0.f }, 0)   // wywo³anie tworzenia Brick
    {
    }

    void initGrid(int kolumny, int wiersze, float szerokoscOkna)
    {
        bloki.clear();

        // wyliczanie rozmiaru bloczków
        float odstep = 2.f;
        rozmiarX = (szerokoscOkna - (kolumny - 1) * odstep) / kolumny;
        rozmiarY = 20.f;

        for (int y = 0; y < wiersze; y++)
        {
            for (int x = 0; x < kolumny; x++)
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

    void update(Ball& pilka)
    {
        // kolizje z pi³k¹
        for (auto& blk : bloki)
        {
            if (!blk.czyZniszczony() &&
                pilka.getGlobalBounds().intersects(blk.getGlobalBounds()))
            {
                blk.trafienie();
                pilka.odbijY();
            }
        }

        // usuwanie zniszczonych
        for (int i = static_cast<int>(bloki.size()) - 1; i >= 0; i--)
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



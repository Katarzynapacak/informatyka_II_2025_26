#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"

class Game
{
public:
    const Paddle& getPaddle() const { return m_paletka; }
    const Ball& getBall()   const { return m_pilka; }
    const std::vector<Brick>& getBlocks() const { return m_bricks.getVector(); }

    Paddle& getPaddle() { return m_paletka; }
    Ball& getBall() { return m_pilka; }
    Bricks& getBricks() { return m_bricks; }


    Game();

    void update(sf::Time dt);
    void render(sf::RenderTarget& target);

private:
    Paddle m_paletka;
    Ball   m_pilka;
    Bricks m_bricks;

    static constexpr int SZEROKOSC = 800;
    static constexpr int WYSOKOSC = 600;
};

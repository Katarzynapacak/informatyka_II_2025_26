#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Bricks.h"
#include "Star.h"

class Game
{
public:
    Game();

    const Paddle& getPaddle() const { return m_paletka; }
    const Ball& getBall() const { return m_pilka; }
    const std::vector<Brick>& getBlocks() const { return m_bricks.getVector(); }
    int  getDestroyedBricks() const { return m_bricks.getDestroyedCount(); }
    int  getInitialBricks() const { return m_bricks.getInitialCount(); }
    bool isBallOutOfBounds(float windowHeight) const;

    Paddle& getPaddle() { return m_paletka; }
    Ball& getBall() { return m_pilka; }
    Bricks& getBricks() { return m_bricks; }

    void reset();
    void update(sf::Time dt);
    void render(sf::RenderTarget& target);

private:
    Paddle m_paletka;
    Ball m_pilka;
    Bricks m_bricks;
    Star m_star; 

    static constexpr int SZEROKOSC = 800;
    static constexpr int WYSOKOSC = 600;
};

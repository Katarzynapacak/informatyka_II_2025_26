#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Bricks.h"

struct BlockData
{
    float x;
    float y;
    float w;
    float h;
    int hp;
};

class GameState
{
private:
    sf::Vector2f m_paddlePos;
    sf::Vector2f m_ballPos;
    sf::Vector2f m_ballVel;

    std::vector<BlockData> m_blocks;

public:
    GameState() = default;

    void capture(const Paddle& paddle,
        const Ball& ball,
        const std::vector<Brick>& blocks);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    void apply(Paddle& paddle,
        Ball& ball,
        Bricks& bricks);
};

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

    // zrzut stanu gry
    void capture(const Paddle& paddle,
        const Ball& ball,
        const std::vector<Brick>& blocks);

    // zapis do pliku
    bool saveToFile(const std::string& filename) const;

    // odczyt z pliku
    bool loadFromFile(const std::string& filename);

    // przepisanie stanu do obiektów gry
    void apply(Paddle& paddle,
        Ball& ball,
        Bricks& bricks);
};

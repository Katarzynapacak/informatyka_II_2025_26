#include "GameState.h"
#include <fstream>

void GameState::capture(const Paddle& paddle,
    const Ball& ball,
    const std::vector<Brick>& blocks)
{
    m_paddlePos = paddle.getPosition();

    m_ballPos = ball.getPosition();
    m_ballVel = ball.getVelocity();

    m_blocks.clear();
    m_blocks.reserve(blocks.size());

    for (const auto& b : blocks)
    {
        BlockData data;
        sf::Vector2f pos = b.getPosition();
        sf::Vector2f size = b.getSize();

        data.x = pos.x;
        data.y = pos.y;
        data.w = size.x;
        data.h = size.y;
        data.hp = b.getHP();

        m_blocks.push_back(data);
    }
}

bool GameState::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    // pozycja paletki
    file << "PADDLE "
        << m_paddlePos.x << " " << m_paddlePos.y << "\n";

    // pozycja pilki
    file << "BALL "
        << m_ballPos.x << " " << m_ballPos.y << " "
        << m_ballVel.x << " " << m_ballVel.y << "\n";

    // rozmiar i stan bloczkow
    file << "BLOCKS_COUNT " << m_blocks.size() << "\n";

    for (const auto& b : m_blocks)
    {
        file << b.x << " " << b.y << " "
            << b.w << " " << b.h << " "
            << b.hp << "\n";
    }

    return true;
}

bool GameState::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string label;

    if (!(file >> label >> m_paddlePos.x >> m_paddlePos.y))
        return false;

    if (!(file >> label >> m_ballPos.x >> m_ballPos.y
        >> m_ballVel.x >> m_ballVel.y))
        return false;

    int count = 0;
    if (!(file >> label >> count))
        return false;

    m_blocks.clear();
    m_blocks.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        BlockData d;
        if (!(file >> d.x >> d.y >> d.w >> d.h >> d.hp))
            return false;

        m_blocks.push_back(d);
    }

    return true;
}

void GameState::apply(Paddle& paddle,
    Ball& ball,
    Bricks& bricks)
{
    paddle.setPosition(m_paddlePos);

    ball.setPosition(m_ballPos);
    ball.setVelocity(m_ballVel);

    std::vector<Brick>& bloki = bricks.getVectorRef();
    bloki.clear();

    for (const auto& d : m_blocks)
    {
        bloki.emplace_back(
            sf::Vector2f(d.x, d.y),
            sf::Vector2f(d.w, d.h),
            d.hp
        );
    }
}

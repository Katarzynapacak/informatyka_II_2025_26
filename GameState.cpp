#include "GameState.h"
#include <fstream>

// --- zrzut stanu gry do GameState ---
void GameState::capture(const Paddle& paddle,
    const Ball& ball,
    const std::vector<Brick>& blocks)
{
    // paletka
    m_paddlePos = paddle.getPosition();

    // pilka
    m_ballPos = ball.getPosition();
    m_ballVel = ball.getVelocity();

    // bloki
    m_blocks.clear();
    m_blocks.reserve(blocks.size());

    for (const auto& b : blocks)
    {
        BlockData data;
        sf::Vector2f pos = b.getPosition();

        data.x = pos.x;
        data.y = pos.y;
        data.hp = b.getHP();

        m_blocks.push_back(data);
    }
}

// --- zapis do pliku tekstowego ---
bool GameState::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    // PADDLE
    file << "PADDLE "
        << m_paddlePos.x << " " << m_paddlePos.y << "\n";

    // BALL
    file << "BALL "
        << m_ballPos.x << " " << m_ballPos.y << " "
        << m_ballVel.x << " " << m_ballVel.y << "\n";

    // BLOKI
    file << "BLOCKS_COUNT " << m_blocks.size() << "\n";

    for (const auto& b : m_blocks)
    {
        file << b.x << " " << b.y << " " << b.hp << "\n";
    }

    return true;
}

// --- odczyt z pliku tekstowego ---
bool GameState::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string label;

    // PADDLE
    if (!(file >> label >> m_paddlePos.x >> m_paddlePos.y))
        return false;

    // BALL
    if (!(file >> label >> m_ballPos.x >> m_ballPos.y
        >> m_ballVel.x >> m_ballVel.y))
        return false;

    // BLOKI
    int count = 0;
    if (!(file >> label >> count))
        return false;

    m_blocks.clear();
    m_blocks.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        BlockData d;
        if (!(file >> d.x >> d.y >> d.hp))
            return false;

        m_blocks.push_back(d);
    }

    return true;
}

// --- przepisanie stanu do obiektów gry ---
void GameState::apply(Paddle& paddle,
    Ball& ball,
    Bricks& bricks)
{
    // ustawienie paletki
    paddle.setPosition(m_paddlePos);

    // ustawienie pilki
    ball.setPosition(m_ballPos);
    ball.setVelocity(m_ballVel);

    // odbudowa bloczkow
    std::vector<Brick>& bloki = bricks.getVectorRef();
    bloki.clear();

    // przyjmujemy takie same wymiary bloczkow jak w initGrid
    const float blockHeight = 20.f;
    const float blockWidth = 50.f; // mo¿esz zmieniæ / policzyæ dok³adniej

    for (const auto& d : m_blocks)
    {
        bloki.emplace_back(
            sf::Vector2f(d.x, d.y),
            sf::Vector2f(blockWidth, blockHeight),
            d.hp
        );
    }
}

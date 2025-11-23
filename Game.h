#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

private:
    sf::RenderWindow m_window;
    sf::Clock        m_deltaClock;

    Paddle m_paletka;
    Ball   m_pilka;
    Bricks m_bricks;

    static constexpr int SZEROKOSC = 800;
    static constexpr int WYSOKOSC = 600;
};

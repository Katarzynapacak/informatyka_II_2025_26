#pragma once
#include <SFML/Graphics.hpp>
#define MAX_LICZBA_POZIOMOW 4

class Menu
{
private:
    sf::Font m_font;
    sf::Text m_items[MAX_LICZBA_POZIOMOW];
    int m_selectedIndex;

public:
    Menu(float width, float height);

    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();

    int getSelectedItemIndex() const { return m_selectedIndex; }
};

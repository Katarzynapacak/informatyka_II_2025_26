#include "Menu.h"

Menu::Menu(float width, float height)
{
    m_selectedIndex = 0;

    if (!m_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
    {
        // blad ladowania czcionki
    }

    // 0 - Nowa gra
    m_items[0].setFont(m_font);
    m_items[0].setFillColor(sf::Color::Cyan);
    m_items[0].setString("Nowa gra");
    m_items[0].setStyle(sf::Text::Bold);
    m_items[0].setPosition(
        sf::Vector2f(width / 3.f, height / (MAX_LICZBA_POZIOMOW + 1.f) * 1.f)
    );

    // 1 - Wczytaj gre
    m_items[1].setFont(m_font);
    m_items[1].setFillColor(sf::Color::White);
    m_items[1].setString("Wczytaj gre");
    m_items[1].setStyle(sf::Text::Regular);
    m_items[1].setPosition(
        sf::Vector2f(width / 3.f, height / (MAX_LICZBA_POZIOMOW + 1.f) * 2.f)
    );

    // 2 - Ostatnie wyniki
    m_items[2].setFont(m_font);
    m_items[2].setFillColor(sf::Color::White);
    m_items[2].setString("Ostatnie wyniki");
    m_items[2].setStyle(sf::Text::Regular);
    m_items[2].setPosition(
        sf::Vector2f(width / 3.f, height / (MAX_LICZBA_POZIOMOW + 1.f) * 3.f)
    );

    // 3 - Wyjscie
    m_items[3].setFont(m_font);
    m_items[3].setFillColor(sf::Color::White);
    m_items[3].setString("Wyjscie");
    m_items[3].setStyle(sf::Text::Regular);
    m_items[3].setPosition(
        sf::Vector2f(width / 3.f, height / (MAX_LICZBA_POZIOMOW + 1.f) * 4.f)
    );
}

void Menu::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < MAX_LICZBA_POZIOMOW; ++i)
    {
        window.draw(m_items[i]);
    }
}

void Menu::moveUp()
{
    m_items[m_selectedIndex].setFillColor(sf::Color::White);
    m_items[m_selectedIndex].setStyle(sf::Text::Regular);

    m_selectedIndex--;
    if (m_selectedIndex < 0)
        m_selectedIndex = MAX_LICZBA_POZIOMOW - 1;

    m_items[m_selectedIndex].setFillColor(sf::Color::Cyan);
    m_items[m_selectedIndex].setStyle(sf::Text::Bold);
}

void Menu::moveDown()
{
    m_items[m_selectedIndex].setFillColor(sf::Color::White);
    m_items[m_selectedIndex].setStyle(sf::Text::Regular);

    m_selectedIndex++;
    if (m_selectedIndex >= MAX_LICZBA_POZIOMOW)
        m_selectedIndex = 0;

    m_items[m_selectedIndex].setFillColor(sf::Color::Cyan);
    m_items[m_selectedIndex].setStyle(sf::Text::Bold);
}

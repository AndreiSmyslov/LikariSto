#ifndef POTWORKI_H
#define POTWORKI_H

#include <SFML/Window.hpp>
#include "SFML/Graphics/Sprite.hpp"
#include <vector>

class Potworki : public sf::Sprite
{
public:
    Potworki();
    ~Potworki();

    static std::vector<std::vector<int>> wektor;
    sf::Vector2f pozycja;

private:
    static int liczba;
};

#endif // POTWORKI_H

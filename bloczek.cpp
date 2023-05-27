#include "bloczek.h"

Bloczek::Bloczek(sf::Vector2f v) : Sprite()
{
    setScale(0.6f, 0.6f);
    setPosition(700, 250);
    predkosc =v;
    move(v);
}

sf::Vector2f Bloczek::szybkosc()
{
    return predkosc;
}

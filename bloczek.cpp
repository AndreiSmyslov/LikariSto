#include "bloczek.h"

Bloczek::Bloczek(sf::Vector2f v) : Sprite()
{
    setScale(0.6f, 0.6f);
    setPosition(660, 250);
    predkosc =v;
    move(v);
}

sf::Vector2f Bloczek::szybkosc()
{
    return predkosc;
}

void Bloczek::aktualizujPredkosc(sf::Vector2f a)
{
    predkosc = a;
}

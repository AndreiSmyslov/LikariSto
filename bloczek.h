#ifndef BLOCZEK_H
#define BLOCZEK_H

#include "SFML/Graphics/Sprite.hpp"

class Bloczek : public sf::Sprite
{
public:
    Bloczek(sf::Vector2f);
    sf::Vector2f szybkosc();
    void aktualizujPredkosc(sf::Vector2f);
private:
    sf::Vector2f predkosc;
};

#endif // BLOCZEK_H

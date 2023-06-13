#ifndef POTWORY_H
#define POTWORY_H
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>


class Potwory
{
public:
    static std::vector<sf::Vector3f> pozycje;
    Potwory(int liczba);
};

#endif // POTWORY_H

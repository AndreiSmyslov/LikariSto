#ifndef POTWORY_H
#define POTWORY_H
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>


class Potwory
{
public:
    // wektor pozycji wszystkich wygenerowanych potworów x i y to pozycje na plaszy a z to kolor
    static std::vector<sf::Vector3f> pozycje;
    // konstruktor przyjmuje ilosc potworkow ktore ma stworzyc
    Potwory(int liczba);
    // dodaje do Macierzy wszystkie potworki
    std::vector<std::vector<int>> DodajDoMacierzy(std::vector<std::vector<int>> mac);
};

#endif // POTWORY_H

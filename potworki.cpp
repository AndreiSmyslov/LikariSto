#include "potworki.h"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Window.hpp>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

std::vector<std::vector<int>> Potworki::wektor;
int Potworki::liczba;

Potworki::Potworki()
    : Sprite()
{
    liczba++;
    int ix;
    int iy;
    bool zajete=false;
    do
        {
        zajete=false;
        std::vector<int> wartoscix = {0, 32, 64, 96, 128, 160, 192, 224};
        std::vector<int> wartosciy = {192, 224, 256, 288, 320, 352, 384, 416, 448, 480};

        // Inicjalizacja generatora liczb pseudolosowych
        std::random_device rd;
        std::mt19937 rng(rd());

        // Utworzenie rozkładu równomiernego dla indeksów
        std::uniform_int_distribution<int> x(0, wartoscix.size() - 1);

        std::uniform_int_distribution<int> y(0, wartosciy.size() - 1);

        // Wylosowanie indeksu
        ix = x(rng);
        iy = y(rng);
        ix=wartoscix[ix]+333;
        iy=wartosciy[iy]+330;
        std::vector<int> wek{ix, iy};

        if (std::find(Potworki::wektor.begin(), Potworki::wektor.end(), std::vector<int>{ix, iy}) != Potworki::wektor.end())
            zajete = true;

    } while(zajete);

    Potworki::wektor.emplace_back(std::vector<int>{ix, iy});
    pozycja=sf::Vector2f (ix, iy);
    setPosition(pozycja);
    setScale(0.6f, 0.6f);
}

Potworki::~Potworki()
{
    liczba--;
}


#include "potwory.h"
#include <SFML/Window.hpp>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;


Potwory::Potwory(int liczba)
{
    float ix;
    float iy;
    float iz;
    bool zajete=false;
    sf::Vector3f poz;

    // Inicjalizacja generatora liczb pseudolosowych
    random_device rd;
    mt19937 rng(rd());

    // Utworzenie rozkładu równomiernego dla indeksów
    // Przedział y nie jest od zera w celu ułatwienia rozgrywki
    uniform_int_distribution<> x(0,7);
    uniform_int_distribution<> y(5,15);
    uniform_int_distribution<> z(1,3);

    for (int i = 0; i<liczba; i++)
    {
    do
        {
        zajete=false;
            // Wylosowanie indeksu
            ix = x(rng);
            iy = y(rng);

             poz.x = ix;
             poz.y = iy;

            if (find(Potwory::pozycje.begin(), Potwory::pozycje.end(), poz) != Potwory::pozycje.end())
                zajete = true;


    } while(zajete);
    Potwory::pozycje.emplace_back(poz);
    }
    // losowanie koloru
    for (auto &lock : pozycje)
    {
        iz = z(rng);
        lock.z = iz+100;
    }

}

vector<vector<int>> Potwory::DodajDoMacierzy(std::vector<std::vector<int>> mac)
{
    for (auto &poz : pozycje)
    {
        mac[poz.x][poz.y] = poz.z;
    }
    return mac;
}

void Potwory::wyczyscPozycje()
{
    pozycje.clear();
}

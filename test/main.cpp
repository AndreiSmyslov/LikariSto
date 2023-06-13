#include "bloczek.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>


using namespace std;

std::vector<std::vector<int>> dodaj_macierze(const std::vector<std::vector<int>>& macierz1, const std::vector<std::vector<int>>& macierz2);

int main() {

    srand(time(NULL));

    // Get a random number

    int CELL_SIZE = 9;
    int COLUMNS = 8;
    int SCREEN_RESIZE = 4;
    int ROWS = 16;

    int pozY = 240;
    int pozX = 300;

//    int spadanie=0;
//    int pozycja =3;

//    bool przesuwanie =true;
//    int numerTekstury1;
//    int numerTekstury2;

    Bloczek bloczek;


    vector<vector<int>> macierz(COLUMNS, vector<int>(ROWS));
    vector<vector<int>> Macierz(COLUMNS, vector<int>(ROWS));


    // create the window
    sf::RenderWindow window(sf::VideoMode(900, 900), "Dr. Mario", sf::Style::Close);


    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));
    rectangle.setFillColor(sf::Color(100, 50, 250));
    //rectangle.setPosition(300, 170);


    sf::Clock clock;


    // run the program as long as the window is open
    while (window.isOpen()) {



        // check all the window's events that were triggered since the last iteration of the loop
        sf::Time time1 = clock.getElapsedTime();
        if (time1.asMilliseconds() > 16.7)
        {
        // Sprawdzanie czy został nacisniety ktorys z klawiszy na klawiaturze
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    bloczek.przesunLewo(Macierz);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    bloczek.przesunPrawo(Macierz);
                }
                if (event.key.code == sf::Keyboard::A) {
                    bloczek.obrocLewo(Macierz);
                }
                if (event.key.code == sf::Keyboard::D) {
                    bloczek.obrocPrawo(Macierz);
                }
                if (event.key.code == sf::Keyboard::Space) {
                    bloczek.Pauza();
                }
            }
        }

        window.clear(sf::Color::Black);


        // Animacja bloczka i tworzenie nowego, gdy funkcja zwroci wartosc true, co bedzie rownowazne z tym ze bloczek stanal na czyms
        sf::Time czas = clock.getElapsedTime();
        if(czas.asMilliseconds()>(300))
        {
            clock.restart();
            if(bloczek.wypelnijMacierz(macierz, COLUMNS, ROWS, Macierz))
            {
                // Dodaje do zmiennej Macierz nowe dwa elementy ktore sa efektem bloczka, ktory dopiero co spaadl
                Macierz = dodaj_macierze(Macierz, macierz);
                // Ustawia wszystkie parametry domyslnie
                bloczek.nowy();
            }
        }

        // Wypelnainie ekranu kwadratami o koloroch zaleznych od liczb z macierzy liczby
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
                if(macierz[i][j]==0)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(100, 100, 100));
                    window.draw(rectangle);
                }

                if(macierz[i][j]==1)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 0, 200));
                    window.draw(rectangle);
                }
                if(macierz[i][j]==2)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 200, 0));
                    window.draw(rectangle);
                }
                if(macierz[i][j]==3)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(200, 0, 0));
                    window.draw(rectangle);
                }
            }
        }

        // Nalozenie na ekran kwadratow kwadratow ktore juz tam byly i staly juz od wczesniejszych rund
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
                if(Macierz[i][j]==1)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 0, 200));
                    window.draw(rectangle);
                }
                if(Macierz[i][j]==2)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 200, 0));
                    window.draw(rectangle);
                }
                if(Macierz[i][j]==3)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(200, 0, 0));
                    window.draw(rectangle);
                }
//                if(Macierz[i][j]!=0)
//                {
//                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
//                    rectangle.setFillColor(sf::Color(0, 0, 200));
//                    window.draw(rectangle);
//                }
            }
        }
        window.display();
        }


    }

    return 0;
}
std::vector<std::vector<int>> dodaj_macierze(const std::vector<std::vector<int>>& macierz1, const std::vector<std::vector<int>>& macierz2) {
    // Sprawdzenie wymiarów macierzy
    if (macierz1.size() != macierz2.size() || macierz1[0].size() != macierz2[0].size()) {
        throw std::invalid_argument("Macierze musza miec takie same wymiary.");
    }

    std::vector<std::vector<int>> suma_macierzy;
    suma_macierzy.reserve(macierz1.size());

    for (size_t i = 0; i < macierz1.size(); ++i) {
        std::vector<int> wiersz;
        wiersz.reserve(macierz1[i].size());

        for (size_t j = 0; j < macierz1[i].size(); ++j) {
            int suma_elementow = macierz1[i][j] + macierz2[i][j];
            wiersz.push_back(suma_elementow);
        }

        suma_macierzy.push_back(wiersz);
    }

    return suma_macierzy;
}

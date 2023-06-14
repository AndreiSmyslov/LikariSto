#include "potwory.h"
#include "bloczek.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

vector<vector<int>> dodaj_macierze(const vector<vector<int>>& macierz1, const vector<vector<int>>& macierz2);
vector<vector<int>> usuwanieBloczkow(vector<vector<int>>& Macierz, int rows, int cols);

int main() {

    srand(time(NULL));

    int CELL_SIZE = 9;
    int COLUMNS = 8;
    int SCREEN_RESIZE = 4;
    int ROWS = 16;

    // przesuniecie planszy
    int pozY = 240;
    int pozX = 300;

    Bloczek bloczek;

    // macierz zawiera w sobie pozycje ruchu tymczasowego bloczka
    vector<vector<int>> macierz(COLUMNS, vector<int>(ROWS));
    // Macierz zawiera w sobie pozycje stałych obiektów
    vector<vector<int>> Macierz(COLUMNS, vector<int>(ROWS));

    // tworzenie okienka
    sf::RenderWindow window(sf::VideoMode(900, 900), "Likaristo", sf::Style::Close);

    // rectangle kwadratem będącym kratka na plaszy
    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));

    // inicjalizacja potworow i dodanie ich do Macierzy
    Potwory potwory(80);
    Macierz = potwory.DodajDoMacierzy(Macierz);

    sf::Clock clock;

    // ustawienie framerejtu
    window.setFramerateLimit(120);

    // petla dziala tak dlugo jak program jest wlaczony
    while (window.isOpen()) {

    // wszystkie klawiaturowe eventy poruszanie sie bloczka, zatrzymywanie gry, zamykanie programu
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    bloczek.przesunLewo(Macierz, macierz);
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    bloczek.przesunPrawo(Macierz, macierz);
                }
                if (event.key.code == sf::Keyboard::A) {
                    bloczek.obrocLewo(Macierz);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    bloczek.obrocPrawo(Macierz);
                }
                if (event.key.code == sf::Keyboard::Space) {
                    bloczek.Pauza();
                }
            }
        }

        window.clear(sf::Color::Black);



    // generowanie nowego bloczka, ruch bloczka, dodanie go do Macierzy, gdy sie zatrzyma
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

    // usuwanie bloczkow jesli spelnione sa warunki
    Macierz = usuwanieBloczkow(Macierz, COLUMNS, ROWS);

    // itercja kolorujaca plansze
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
    // czesc iteracji kolorujaca nieaktywne szare kartki na bazie macierzy
                if(macierz[i][j]==0)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(100, 100, 100));
                    window.draw(rectangle);
                }
    // czesc iteracji kolorujaca pola ruchu bloczka na bazie macierzy
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

        ///

    //iteracja kolorujaca wszystkie pola będące w Macierzy
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
                if(Macierz[i][j]==1 || Macierz[i][j]==101)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 0, 200));
                    window.draw(rectangle);
                }
                if(Macierz[i][j]==2 || Macierz[i][j]==102)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(0, 200, 0));
                    window.draw(rectangle);
                }
                if(Macierz[i][j]==3 || Macierz[i][j]==103)
                {
                    rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                    rectangle.setFillColor(sf::Color(200, 0, 0));
                    window.draw(rectangle);
                }
            }
        }

        window.display();


    }

    return 0;
}


vector<vector<int>> dodaj_macierze(const vector<vector<int>>& macierz1, const vector<vector<int>>& macierz2) {
    // Sprawdzenie wymiarów macierzy
    if (macierz1.size() != macierz2.size() || macierz1[0].size() != macierz2[0].size()) {
        throw invalid_argument("Macierze musza miec takie same wymiary.");
    }

    vector<vector<int>> suma_macierzy;
    suma_macierzy.reserve(macierz1.size());

    for (size_t i = 0; i < macierz1.size(); ++i) {
        vector<int> wiersz;
        wiersz.reserve(macierz1[i].size());

        for (size_t j = 0; j < macierz1[i].size(); ++j) {
            int suma_elementow = macierz1[i][j] + macierz2[i][j];
            wiersz.push_back(suma_elementow);
        }

        suma_macierzy.push_back(wiersz);
    }

    return suma_macierzy;
}

vector<vector<int>> usuwanieBloczkow(vector<vector<int>>& Macierz, int rows, int cols)
{
  vector<vector<int>> tempMacierz = Macierz;

  for (int i=0; i< rows; i++)
  {
      for(int j=0; j < cols; j++)
      {
          while(tempMacierz[i][j]>100)
          {
            tempMacierz[i][j] -= 100;
          }

      }
  }
  // Sprawdź wiersze
  for (auto& row : tempMacierz) {
    int licznik = 1;
    int poprzedniNumer = row[0];
    for (size_t i = 1; i < row.size(); i++) {
      if (row[i] == poprzedniNumer) {
        licznik++;
        if (licznik >= 4) {
          for (size_t j = i - licznik + 1; j <= i; j++) {
            Macierz[&row - &tempMacierz[0]][j] = 0;
          }
        }
      } else {
        licznik = 1;
        poprzedniNumer = row[i];
      }
    }
  }

  // Sprawdź kolumny
  for (int j = 0; j < cols; j++) {
    int licznik = 1;
    int poprzedniNumer = tempMacierz[0][j];
    for (int i = 1; i < rows; i++) {
      if (tempMacierz[i][j] == poprzedniNumer) {
        licznik++;
        if (licznik >= 4) {
          for (int k = i - licznik + 1; k <= i; k++) {
            Macierz[k][j] = 0;
          }
        }
      } else {
        licznik = 1;
        poprzedniNumer = tempMacierz[i][j];
      }
    }
  }
  return Macierz;
}

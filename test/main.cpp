#include "potwory.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>




using namespace std;




int main() {

    srand(time(NULL));

    // Get a random number

    int CELL_SIZE = 9;
    int COLUMNS = 8;
    int SCREEN_RESIZE = 4;
    int ROWS = 16;

    int pozY = 240;
    int pozX = 300;

    int spadanie=0;
    int pozycja =3;

    vector<vector<int>> macierz(COLUMNS, vector<int>(ROWS));

    // create the window
    sf::RenderWindow window(sf::VideoMode(900, 900), "Likaristo", sf::Style::Close);


    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));
    rectangle.setFillColor(sf::Color(100, 50, 250));
    //rectangle.setPosition(300, 170);

    // inicjalizacja potworow
    Potwory potwory(88);
//    for (auto &potwo : potwory.pozycje)
//    {
//        cout << potwo.x << " " << potwo.y << " " << potwo.z << endl;
//    }



    sf::Clock clock;

    window.setFramerateLimit(30);
    // run the program as long as the window is open
    while (window.isOpen()) {



        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if(pozycja>0)
                        pozycja--;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if(pozycja<6)
                        pozycja++;
                }
            }
        }

        window.clear(sf::Color::Black);

        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
                sf::Time czas = clock.getElapsedTime();
                if(czas.asMilliseconds()>(600))
                {
                spadanie++;
                czas = clock.restart();
                //spadanie=0;
                }
                rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                if((i == pozycja && j==spadanie) || (i == pozycja+1 && j == spadanie))
                {
                rectangle.setFillColor(sf::Color(250, 0, 0));
                //macierz[i][j] = ;
                }
                for (auto &potwo : potwory.pozycje)
                {

                    if((i == potwo.x && j==potwo.y))
                    {
                        if(potwo.z == 1.f)
                        {
                            rectangle.setFillColor(sf::Color(0, 250, 0));
                        }
                        else if(potwo.z == 2.f)
                        {
                            rectangle.setFillColor(sf::Color(250, 250, 0));
                        }
                        else
                        {
                            rectangle.setFillColor(sf::Color(250, 0, 250));
                        }

                    }

                }

                window.draw(rectangle);
                rectangle.setFillColor(sf::Color(100, 50, 250));

            }
        }
        window.display();



    }

    return 0;
}

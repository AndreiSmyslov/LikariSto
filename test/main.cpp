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
    sf::RenderWindow window(sf::VideoMode(900, 900), "Tetris", sf::Style::Close);


    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));
    rectangle.setFillColor(sf::Color(100, 50, 250));
    //rectangle.setPosition(300, 170);


    sf::Clock clock;




    window.setFramerateLimit(30);
    // run the program as long as the window is open
    while (window.isOpen()) {



        // check all the window's events that were triggered since the last iteration of the loop
        sf::Time time1 = clock.getElapsedTime();
        if (time1.asMilliseconds() > 16.7)
        {


        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    pozycja--;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    pozycja++;
                }
            }
        }




        // clear the window with black color
        window.clear(sf::Color::Black);


        // draw everything here...
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {

                rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                if (event.type == sf::Event::MouseButtonPressed) {
                    if(event.mouseButton.button == sf::Mouse::Left) {

                        sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        int cell_x = mouse_position.x / CELL_SIZE / SCREEN_RESIZE;
                        int cell_y = mouse_position.y /100;
                        //cout << mouse_position.x << " " << mouse_position.y << endl;
                        if(rectangle.getGlobalBounds().contains(mouse_position))
                            rectangle.setFillColor(sf::Color(250, 0, 0));
                        //??
                        //macierz[cell_x][cell_y] = 1 - macierz[cell_x][cell_y];
                        }
                        }
                window.draw(rectangle);
                rectangle.setFillColor(sf::Color(100, 50, 250));

            }
        }




        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {
                sf::Time czas = clock.getElapsedTime();
//                if(czas.asMilliseconds()>(600))
//                {
                spadanie++;
                //time1 = clock.restart();
                //spadanie=0;
                //}
                rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
                if((i == pozycja && j==spadanie) || (i == pozycja+1 && j == spadanie))
                {
                rectangle.setFillColor(sf::Color(250, 0, 0));
                }


                window.draw(rectangle);
                rectangle.setFillColor(sf::Color(100, 50, 250));

            }
        }
        window.display();
        }


    }

    return 0;
}

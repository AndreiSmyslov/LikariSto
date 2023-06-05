#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>


using namespace std;


int main() {

    srand(time(NULL));

    // Get a random number

    int CELL_SIZE = 8;
    int COLUMNS = 10;
    int SCREEN_RESIZE = 4;
    int ROWS = 20;

    vector<vector<int>> macierz(COLUMNS, vector<int>(ROWS));

    // create the window
    sf::RenderWindow window(sf::VideoMode( CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);


    // create some shapes
    sf::CircleShape circle(100.0);
    circle.setPosition(100.0, 300.0);
    circle.setFillColor(sf::Color(100, 250, 50));

    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));
    rectangle.setFillColor(sf::Color(100, 50, 250));

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0.0, 0.0));
    triangle.setPoint(1, sf::Vector2f(0.0, 100.0));
    triangle.setPoint(2, sf::Vector2f(140.0, 40.0));
    triangle.setOutlineColor(sf::Color::Red);
    triangle.setOutlineThickness(5);
    triangle.setPosition(600.0, 100.0);

    sf::Clock clock;


//    float rectangle_velocity_x = 50;
//    float rectangle_velocity_y = 150;
//    float rectangle_angular_velocity = 3;



    // run the program as long as the window is open
    while (window.isOpen()) {



        // check all the window's events that were triggered since the last iteration of the loop
        sf::Time time1 = clock.getElapsedTime();
        if (time1.asMilliseconds() > 16.7)
        {


//            sf::FloatRect rectangle_bounds = rectangle.getGlobalBounds();
//            std::cout << rectangle_bounds.top << " " << rectangle_bounds.left << " " ;
//            std::cout << rectangle_bounds.width << " " << rectangle_bounds.height << std::endl;
////
//            if(rectangle_bounds.left <0)
//            {
//                int randomR = rand()%256;
//                int randomG = rand()%256;
//                int randomB = rand()%256;
//             rectangle_velocity_x = abs(rectangle_velocity_x);
//             rectangle.setFillColor(sf::Color(randomR, randomG, randomB));
//            }
//            else if(rectangle_bounds.left+rectangle_bounds.width >800)
//            {
//                int randomR = rand()%256;
//                int randomG = rand()%256;
//                int randomB = rand()%256;
//             rectangle_velocity_x = -abs(rectangle_velocity_x);
//             rectangle.setFillColor(sf::Color(randomR, randomG, randomB));
//            }
//            if(rectangle_bounds.top<0)
//            {
//                int randomR = rand()%256;
//                int randomG = rand()%256;
//                int randomB = rand()%256;
//                rectangle_velocity_y = abs(rectangle_velocity_y);
//                rectangle.setFillColor(sf::Color(randomR, randomG, randomB));
//            }
//            else if(rectangle_bounds.top+rectangle_bounds.height > 600)
//            {
//                int randomR = rand()%256;
//                int randomG = rand()%256;
//                int randomB = rand()%256;
//                rectangle_velocity_y = -abs(rectangle_velocity_y);
//                rectangle.setFillColor(sf::Color(randomR, randomG, randomB));
//            }






//            float rec_s_x = rectangle_velocity_x/time1.asMilliseconds();
//            float rec_s_y = rectangle_velocity_y/time1.asMilliseconds();
//            rectangle.move(rec_s_x,rec_s_y);
//            rectangle.rotate(rectangle_angular_velocity);
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }




        //sf::Time elapsed = clock.restart();
        // clear the window with black color
        window.clear(sf::Color::Black);


        // draw everything here...
        for (int i=0; i< COLUMNS; i++)
        {
            for(int j=0; j < ROWS; j++)
            {

                rectangle.setPosition(CELL_SIZE * i * 4, CELL_SIZE * j *4);
                if (event.type == sf::Event::MouseButtonPressed) {
                    if(event.mouseButton.button == sf::Mouse::Left) {

                        sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        int cell_x = mouse_position.x / CELL_SIZE / SCREEN_RESIZE;
                        int cell_y = mouse_position.y /100;
                        cout << mouse_position.x << " " << mouse_position.x << endl;
                        rectangle.setFillColor(sf::Color(250, 0, 0));
                        //??
                        //macierz[cell_x][cell_y] = 1 - macierz[cell_x][cell_y];
                        }
                        }
                window.draw(rectangle);
                rectangle.setFillColor(sf::Color(100, 50, 250));

            }
        }
        //cout << time1.asMicroseconds() << endl;
        time1 = clock.restart();
        window.display();
        // end the current frame
        }


    }

    return 0;
}

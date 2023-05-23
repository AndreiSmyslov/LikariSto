#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>



int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "DR. Mario");
    window.setFramerateLimit(60);

    // W zmiennej font mamy czcionke, jezeli chcesz polskie znaki to przed tekstem daj 'L'
    // np. sf::Text text(L"śćżźą", font, 24);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }

    int etapGry = 1; // 1 - ekran poczatkowy z wyborem ustawien, 2 - ekran gry, 3 - ekran koncowy z wynikiem
    int poziomTrudnosci=0;
    bool isDragging = false;
    int value = 1;
    int pozycjaSliderX = 200;
    int pozycjaSliderY = 290;

    while (window.isOpen()) {

        /* Sprawdzenie Eventów: *******************************************************/
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (etapGry == 1) {
                    sf::RectangleShape rectangle1(sf::Vector2f(145.0, 45.0));
                    rectangle1.setPosition(200.0, 460.0);

                    sf::RectangleShape rectangle2(sf::Vector2f(170.0, 45.0));
                    rectangle2.setPosition(345.0, 460.0);

                    sf::RectangleShape rectangle3(sf::Vector2f(170.0, 45.0));
                    rectangle3.setPosition(515.0, 460.0);

                    if (rectangle1.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=1;
                    }
                    else if (rectangle2.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=2;
                    }
                    else if (rectangle3.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=3;
                    }
                    sf::RectangleShape track(sf::Vector2f(400, 10));
                    track.setPosition(200, 300);
                    sf::RectangleShape slider(sf::Vector2f(20, 40));
                    slider.setPosition(pozycjaSliderX, pozycjaSliderY);
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        if (slider.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
                        {
                            isDragging = true;
                        }
                    }
                }
                sf::RectangleShape przyciskGraj(sf::Vector2f(170, 80));
                przyciskGraj.setPosition(550, 590);
                if ((przyciskGraj.getGlobalBounds().contains(mousePosition)) && (poziomTrudnosci!=0)) {
                    etapGry=2;
                }

            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDragging = false;
                }
            }
        }

        /* Etap pierwszy, wybór trudności gry: *******************************************************/
        if(etapGry==1)
        {
            std::vector<std::unique_ptr<sf::Drawable>> shapes;

            // Tworzenie kształtów, tekstów i dodawanie ich do wektora:
            sf::RectangleShape rectangle1(sf::Vector2f(145.0, 45.0));
            rectangle1.setPosition(200.0, 460.0);
            rectangle1.setFillColor(sf::Color::Green);
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(rectangle1));

            sf::RectangleShape rectangle2(sf::Vector2f(170.0, 45.0));
            rectangle2.setPosition(345.0, 460.0);
            rectangle2.setFillColor(sf::Color::Blue);
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(rectangle2));

            sf::RectangleShape rectangle3(sf::Vector2f(170.0, 45.0));
            rectangle3.setPosition(515.0, 460.0);
            rectangle3.setFillColor(sf::Color::Red);
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(rectangle3));

            sf::RectangleShape track(sf::Vector2f(400, 10));
            track.setFillColor(sf::Color::White);
            track.setPosition(200, 300);
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(track));

            sf::RectangleShape slider(sf::Vector2f(20, 40));
            slider.setFillColor(sf::Color::Red);
            slider.setPosition(pozycjaSliderX, pozycjaSliderY);

            sf::Text tytul("Dr. Mario", font, 70);
            tytul.setPosition(250.f, 60.f);
            tytul.setFillColor(sf::Color::Red);
            tytul.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
            tytul.setOutlineColor(sf::Color::Yellow);
            tytul.setOutlineThickness(5);
            shapes.emplace_back(std::make_unique<sf::Text>(tytul));

            sf::Text tryb(L"Ilość Wirusów:", font, 30);
            tryb.setPosition(210.f, 230.f);
            shapes.emplace_back(std::make_unique<sf::Text>(tryb));

            sf::Text szybkosc(L"Tryb gry:", font, 30);
            szybkosc.setPosition(210.f, 390.f);
            shapes.emplace_back(std::make_unique<sf::Text>(szybkosc));

            sf::Text tryby(L"1. Tryb łatwy  2. Tryb średni  3. Tryb trudny", font, 24);
            tryby.setPosition(200.f, 465.f);
            shapes.emplace_back(std::make_unique<sf::Text>(tryby));

            // Wyświetlanie opcji która została wybrana klikając przycisk:
            if(poziomTrudnosci==1)
            {
                sf::Text poziom(L"Łatwy", font, 25);
                poziom.setPosition(440.f, 390.f);
                shapes.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==2)
            {
                sf::Text poziom(L"Średni", font, 25);
                poziom.setPosition(440.f, 390.f);
                shapes.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==3)
            {
                sf::Text poziom(L"Trudny", font, 25);
                poziom.setPosition(440.f, 390.f);
                shapes.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            // Przesuwanie suwakiem jeżeli został kliknięty
            if (isDragging)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                float mouseX = static_cast<float>(mousePosition.x);

                if (mouseX < 200)
                    mouseX = 200;
                else if (mouseX > 600)
                    mouseX = 600;

                value = (mouseX - 200) / 21 + 1; // Zmiana pozycji myszy na wartość suwaka (1-20)
                slider.setPosition(mouseX, 290);
                pozycjaSliderX=mouseX;
                pozycjaSliderY=290;
            }
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(slider));

            // Tworzenie kształtów, tekstów i dodawanie ich do wektora c.d.:
            sf::Text ilosc(std::to_string(value), font, 30);
            ilosc.setPosition(460.f, 230.f);
            shapes.emplace_back(std::make_unique<sf::Text>(ilosc));

            sf::RectangleShape przyciskGraj(sf::Vector2f(170, 80));
            przyciskGraj.setFillColor(sf::Color::Red);
            przyciskGraj.setPosition(550, 590);
            shapes.emplace_back(std::make_unique<sf::RectangleShape>(przyciskGraj));

            sf::Text graj(L"GRAJ", font, 45);
            graj.setPosition(570.f, 600.f);
            graj.setFillColor(sf::Color::Yellow);
            shapes.emplace_back(std::make_unique<sf::Text>(graj));

            // Wyswietlanie wszystkiego z wektora:
            window.clear(sf::Color::Black);
            for(const auto &s : shapes) {
                window.draw(*s);
            }

            window.display();
        }
        /* Etap drugi: *******************************************************/
        if(etapGry==2)
        {
            std::vector<std::unique_ptr<sf::Drawable>> shapes;
            window.clear(sf::Color::Black);
            for(const auto &s : shapes) {
                window.draw(*s);
            }

            window.display();
        }
    }

    return 0;
}

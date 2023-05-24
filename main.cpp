#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>



int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "DR. Mario");
    window.setFramerateLimit(60);

    // W zmiennej czcionka mamy czcionke, jezeli chcesz polskie znaki to przed tekstem daj 'L'
    // np. sf::Text text(L"śćżźą", font, 24);
    sf::Font czcionka;
    if (!czcionka.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }
    sf::Clock clock;

    int etapGry = 1; // 1 - ekran poczatkowy z wyborem ustawien, 2 - ekran gry, 3 - ekran koncowy z wynikiem
    int poziomTrudnosci=0;
    bool suwak = false;
    bool gra = true;
    int value = 1;
    int pozycjaSuwakX = 200;
    int pozycjaSuwakY = 290;


    sf::Texture doktor1;
    if (!doktor1.loadFromFile("doktor1.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;
    }

    sf::Texture doktor2;
    if (!doktor2.loadFromFile("doktor2.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 2." << std::endl;
        return -1;
    }

    sf::Texture doktor3;
    if (!doktor3.loadFromFile("doktor3.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 3." << std::endl;
        return -1;
    }

    sf::Texture doktor4;
    if (!doktor4.loadFromFile("doktor4.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 4." << std::endl;
        return -1;
    }

    sf::Texture doktor5;
    if (!doktor5.loadFromFile("doktor5.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 5." << std::endl;
        return -1;
    }

    sf::Sprite doktorNoga;
    doktorNoga.setTexture(doktor1);
    doktorNoga.setPosition(380, 60);

    sf::Sprite doktorRzut;
    doktorRzut.setTexture(doktor3);
    doktorRzut.setPosition(380, 60);

    sf::Texture tlo1;
        if (!tlo1.loadFromFile("tlo1.jpg")) {
            std::cerr << "Could not load texture" << std::endl;
            return 1;
        }

    sf::Texture tlo2;
    if (!tlo2.loadFromFile("tlo2.jpg")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

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
                    sf::RectangleShape prostokat1(sf::Vector2f(145.0, 45.0));
                    prostokat1.setPosition(200.0, 460.0);

                    sf::RectangleShape prostokat2(sf::Vector2f(170.0, 45.0));
                    prostokat2.setPosition(345.0, 460.0);

                    sf::RectangleShape prostokat3(sf::Vector2f(170.0, 45.0));
                    prostokat3.setPosition(515.0, 460.0);

                    if (prostokat1.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=1;
                    }
                    else if (prostokat2.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=2;
                    }
                    else if (prostokat3.getGlobalBounds().contains(mousePosition)) {
                        poziomTrudnosci=3;
                    }
                    sf::RectangleShape track(sf::Vector2f(400, 10));
                    track.setPosition(200, 300);
                    sf::RectangleShape slider(sf::Vector2f(20, 40));
                    slider.setPosition(pozycjaSuwakX, pozycjaSuwakY);
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2i pozycjaMyszy = sf::Mouse::getPosition(window);
                        if (slider.getGlobalBounds().contains(static_cast<sf::Vector2f>(pozycjaMyszy)))
                        {
                            suwak = true;
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
                    suwak = false;
                }
            }
        }

        /* Etap pierwszy, wybór trudności gry: *******************************************************/
        if(etapGry==1)
        {

            std::vector<std::unique_ptr<sf::Drawable>> wszystkieKsztalty;

            // Tworzenie kształtów, tekstów i dodawanie ich do wektora:

                tlo1.setRepeated(true);
                sf::Sprite sprite;
                sprite.setTexture(tlo1);
                //sprite.setScale(0.3, 0.3);
                sprite.setTextureRect(sf::IntRect(0, 0, 900, 900));
                wszystkieKsztalty.emplace_back(std::make_unique<sf::Sprite>(sprite));

            sf::RectangleShape prostokat1(sf::Vector2f(145.0, 45.0));
            prostokat1.setPosition(200.0, 460.0);
            prostokat1.setFillColor(sf::Color::Green);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(prostokat1));

            sf::RectangleShape prostokat2(sf::Vector2f(170.0, 45.0));
            prostokat2.setPosition(345.0, 460.0);
            prostokat2.setFillColor(sf::Color::Blue);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(prostokat2));

            sf::RectangleShape prostokat3(sf::Vector2f(170.0, 45.0));
            prostokat3.setPosition(515.0, 460.0);
            prostokat3.setFillColor(sf::Color::Red);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(prostokat3));

            sf::RectangleShape track(sf::Vector2f(400, 10));
            track.setFillColor(sf::Color::White);
            track.setPosition(200, 300);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(track));

            sf::RectangleShape slider(sf::Vector2f(20, 40));
            slider.setFillColor(sf::Color::Red);
            slider.setPosition(pozycjaSuwakX, pozycjaSuwakY);

            sf::Text tytul("Dr. Mario", czcionka, 70);
            tytul.setPosition(250.f, 60.f);
            tytul.setFillColor(sf::Color::Red);
            tytul.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
            tytul.setOutlineColor(sf::Color::Yellow);
            tytul.setOutlineThickness(5);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(tytul));

            sf::Text tryb(L"Ilość Wirusów:", czcionka, 30);
            tryb.setPosition(210.f, 230.f);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(tryb));

            sf::Text szybkosc(L"Tryb gry:", czcionka, 30);
            szybkosc.setPosition(210.f, 390.f);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(szybkosc));

            sf::Text tryby(L"1. Tryb łatwy  2. Tryb średni  3. Tryb trudny", czcionka, 24);
            tryby.setPosition(200.f, 465.f);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(tryby));

            // Wyświetlanie opcji która została wybrana klikając przycisk:
            if(poziomTrudnosci==1)
            {
                sf::Text poziom(L"Łatwy", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==2)
            {
                sf::Text poziom(L"Średni", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==3)
            {
                sf::Text poziom(L"Trudny", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(poziom));
            }

            // Przesuwanie suwakiem jeżeli został kliknięty
            if (suwak)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                float mouseX = static_cast<float>(mousePosition.x);

                if (mouseX < 200)
                    mouseX = 200;
                else if (mouseX > 600)
                    mouseX = 600;

                value = (mouseX - 200) / 21 + 1; // Zmiana pozycji myszy na wartość suwaka (1-20)
                slider.setPosition(mouseX, 290);
                pozycjaSuwakX=mouseX;
                pozycjaSuwakY=290;
            }
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(slider));

            // Tworzenie kształtów, tekstów i dodawanie ich do wektora c.d.:
            sf::Text ilosc(std::to_string(value), czcionka, 30);
            ilosc.setPosition(460.f, 230.f);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(ilosc));

            sf::RectangleShape przyciskGraj(sf::Vector2f(170, 80));
            przyciskGraj.setFillColor(sf::Color::Red);
            przyciskGraj.setPosition(550, 590);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::RectangleShape>(przyciskGraj));

            sf::Text graj(L"GRAJ", czcionka, 45);
            graj.setPosition(570.f, 600.f);
            graj.setFillColor(sf::Color::Yellow);
            wszystkieKsztalty.emplace_back(std::make_unique<sf::Text>(graj));

            // Wyswietlanie wszystkiego z wektora:
            window.clear(sf::Color::Black);
            for(const auto &s : wszystkieKsztalty) {
                window.draw(*s);
            }

            window.display();
        }
        /* Etap drugi: *******************************************************/
        if(etapGry==2)
        {
            std::vector<std::unique_ptr<sf::Drawable>> shapes;

            tlo2.setRepeated(true);
            sf::Sprite sprite;
            sprite.setTexture(tlo2);
            //sprite.setScale(0.3, 0.3);
            sprite.setTextureRect(sf::IntRect(0, 0, 900, 900));
            shapes.emplace_back(std::make_unique<sf::Sprite>(sprite));

            if(gra)
            {
                // Czas animacji i opóźnienie między klatkami
                sf::Time animationTime = sf::seconds(0.6f);
                sf::Time elapsed = clock.getElapsedTime();

                // Przełączanie między teksturami co określony czas
                if (elapsed >= animationTime)
                {
                    if (doktorRzut.getTexture() == &doktor3)
                        doktorRzut.setTexture(doktor4);

                    else if (doktorRzut.getTexture() == &doktor5)
                        doktorRzut.setTexture(doktor3);

                    else
                        doktorRzut.setTexture(doktor5);
                    clock.restart();
                }
                shapes.emplace_back(std::make_unique<sf::Sprite>(doktorRzut));
            }
            else
            {
                // Czas animacji i opóźnienie między klatkami
                sf::Time animationTime = sf::seconds(0.6f);
                sf::Time elapsed = clock.getElapsedTime();

                // Przełączanie między teksturami co określony czas
                if (elapsed >= animationTime)
                {
                    if (doktorNoga.getTexture() == &doktor1)
                        doktorNoga.setTexture(doktor2);
                    else
                        doktorNoga.setTexture(doktor1);
                    clock.restart();
                }
                shapes.emplace_back(std::make_unique<sf::Sprite>(doktorNoga));

            }

            window.clear(sf::Color::Black);
            for(const auto &s : shapes) {
                window.draw(*s);
            }

            window.display();
        }
    }

    return 0;
}

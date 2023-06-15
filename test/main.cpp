#include "potwory.h"
#include "bloczek.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix dodaj_macierze(const Matrix& macierz1, const Matrix& macierz2);
Matrix usuwanieBloczkow(Matrix& Macierz, int rows, int cols, int &liczbaZbic);
bool czyWygrany(int COLUMNS, int ROWS, Matrix Macierz);
bool czyPrzegrany(Matrix Macierz);
void wczytajProstokaty(vector<unique_ptr<sf::Drawable>> &ksztalty);
int licznik(Matrix Macierz, int, int);
Matrix opadanieBloczkow(Matrix &macierz, int rows, int cols);
vector<vector<sf::Sprite>> stworzSprajty(int cols, int rows);



int main() {

    srand(time(NULL));

    // startowe informacje o planszy
    int CELL_SIZE = 9;
    int COLUMNS = 8;
    int SCREEN_RESIZE = 4;
    int ROWS = 16;

    // przesuniecie planszy
    int pozY = 240;
    int pozX = 200;


    std::vector<int> liczby;

    // set uzyty przy okreslaniu ktore bloczki maja nie spadac w glownej petli
    set<int> spadanieStop = {101,102,103};

    // W zmiennej czcionka mamy czcionke, jezeli chcesz polskie znaki to przed tekstem daj 'L'
    // np. sf::Text text(L"śćżźą", font, 24);
    sf::Font czcionka;
    if (!czcionka.loadFromFile("textury/ARCADECLASSIC.ttf"))
    {
        return EXIT_FAILURE;
    }

    int etapGry=1;

    int pozycjaSuwakX = 200;
    int pozycjaSuwakY = 286;

    int value = 1;
    bool suwak = false;

    int poziomTrudnosci;

    int liczbaPotworkow;
    int liczbaProb=0;
    int liczbaZbic=0;
    int wynikKoncowy=0;

    bool wygrany=false;
    bool przegrany=false;
    bool zapis = true;


    //zmienne do animowania potworow
    sf::Clock clock2;
    float animationTime = 0.6f;
    sf::Texture potworFioletowyTextures[2];
    sf::Texture potworMorskiTextures[2];
    sf::Texture potworZoltyTextures[2];

    Bloczek bloczek;

    // macierz zawiera w sobie pozycje ruchu tymczasowego bloczka
    Matrix macierz(COLUMNS, vector<int>(ROWS));
    // Macierz zawiera w sobie pozycje obiektów ktorymi gracz nie porusza
    Matrix Macierz(COLUMNS, vector<int>(ROWS));
    // Sprajty zawierają w sobie pozycje kolcków planszy
    vector<vector<sf::Sprite>> Sprajty = stworzSprajty(COLUMNS, ROWS);
    // Tekstury zawierają w sobie tekstury ktore sa cyklicznie zamienianie w podczas ich rysowania
    vector<sf::Texture> Tekstury;

    // tworzenie okienka
    sf::RenderWindow window(sf::VideoMode(900, 900), "Likaristo", sf::Style::Close);

    // rectangle kwadratem będącym kratka na plaszy
    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));


    // wektor spritów i inicjalizacja tekstów wraz z niektrórymi sprite'ami
    std::vector<std::unique_ptr<sf::Sprite>> sprajtyMain;
    std::vector<std::unique_ptr<sf::Sprite>> sprajtyKon;

    sf::Texture tloTexture;
    if (!tloTexture.loadFromFile("textury/tlo_main_gra_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1; }
    tloTexture.setRepeated(true);
    sf::Sprite tloSprite;
    tloSprite.setTexture(tloTexture);
    tloSprite.setTextureRect(sf::IntRect(0, 0, 900, 900));
    sprajtyMain.emplace_back(make_unique<sf::Sprite>(tloSprite));
    sf::Texture butelkaTexture;
    if (!butelkaTexture.loadFromFile("textury/butelka_main_gra_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;}
    sf::Sprite butelkaSprite;
    butelkaSprite.setTexture(butelkaTexture);
    butelkaSprite.setTextureRect(sf::IntRect(0, 0, 80, 176));
    butelkaSprite.setPosition(pozX-36, pozY-180);
    butelkaSprite.setScale(4.5, 4.5);
    sprajtyMain.emplace_back(make_unique<sf::Sprite>(butelkaSprite));

    sf::Texture tloKonTexture1;
    if (!tloKonTexture1.loadFromFile("textury/tlo_win_kon_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1; }
    tloKonTexture1.setRepeated(true);
    sf::Sprite tloKonSpriteW;
    tloKonSpriteW.setTexture(tloKonTexture1);
    tloKonSpriteW.setTextureRect(sf::IntRect(0, 0, 900, 900));
    sf::Texture tloKonTexture2;
    if (!tloKonTexture2.loadFromFile("textury/tlo_lose_kon_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1; }
    tloKonTexture2.setRepeated(true);
    sf::Sprite tloKonSpriteL;
    tloKonSpriteL.setTexture(tloKonTexture2);
    tloKonSpriteL.setTextureRect(sf::IntRect(0, 0, 900, 900));

    sf::Texture pigula_zolta;
    if (!pigula_zolta.loadFromFile("textury/pigula_zolta_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    sf::Texture pigula_fioletowa;
    if (!pigula_fioletowa.loadFromFile("textury/pigula_fioletowa_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 2." << std::endl;
        return -1;}
    sf::Texture pigula_morska;
    if (!pigula_morska.loadFromFile("textury/pigula_morski_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 3." << std::endl;
        return -1;}
    sf::Texture bloczekTexture;
    if (!bloczekTexture.loadFromFile("textury/bloczek_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1; }

    if (!potworZoltyTextures[0].loadFromFile("textury/potwor_zolty_1_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    if (!potworZoltyTextures[1].loadFromFile("textury/potwor_zolty_2_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    if (!potworFioletowyTextures[0].loadFromFile("textury/potwor_fioletowy_1_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    if (!potworFioletowyTextures[1].loadFromFile("textury/potwor_fioletowy_2_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    if (!potworMorskiTextures[0].loadFromFile("textury/potwor_morski_1_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}
    if (!potworMorskiTextures[1].loadFromFile("textury/potwor_morski_2_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;}








    sf::Clock clock;
    sf::Clock clock1;

    // ustawienie framerejtu
    window.setFramerateLimit(120);

    // petla dziala tak dlugo jak program jest wlaczony
    while (window.isOpen()) {

        // wszystkie klawiaturowe eventy poruszanie sie bloczka, zatrzymywanie gry, zamykanie programu
        sf::Event event;
        while (window.pollEvent(event)) {

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
                    // inicjalizacja potworow i dodanie ich do Macierzy
                    Potwory w(1);
                    w.wyczyscPozycje();
                    Potwory potwory(value);
                    Macierz = potwory.DodajDoMacierzy(Macierz);
                    liczbaProb=0;
                    zapis=true;
                    liczbaZbic=0;
                    wynikKoncowy=0;
                }

                if (przegrany || wygrany) {
                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 550.0);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 550.0);

                    if (prostokat1.getGlobalBounds().contains(mousePosition)) {
                        wygrany=false;
                        clock1.restart();
                        przegrany=false;
                        for (int i=0; i< COLUMNS; i++)
                        {
                            for(int j=0; j < ROWS; j++)
                            {
                                Macierz[i][j]=0;
                            }
                        }
                        etapGry=1;
                    }
                    else if (prostokat2.getGlobalBounds().contains(mousePosition)) {
                        window.close();
                    }
                }
            }

            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    suwak = false;
                }
            }


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


        if(etapGry==1)
        {
            vector<unique_ptr<sf::Drawable>> wszystkieKsztalty;

            wczytajProstokaty(wszystkieKsztalty);

            sf::Text tryb(L"Ilosc Wirusow", czcionka, 30);
            tryb.setPosition(210.f, 230.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(tryb));

            sf::Text szybkosc(L"Tryb gry", czcionka, 30);
            szybkosc.setPosition(210.f, 390.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(szybkosc));

            sf::Text tytul("Likaristo", czcionka, 70);
            tytul.setPosition(250.f, 60.f);
            tytul.setFillColor(sf::Color::Red);
            tytul.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
            tytul.setOutlineColor(sf::Color::Yellow);
            tytul.setOutlineThickness(5);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(tytul));

            sf::RectangleShape slider(sf::Vector2f(20, 40));
            slider.setFillColor(sf::Color::Red);
            slider.setPosition(pozycjaSuwakX, pozycjaSuwakY);

            sf::Text tryby(L"             Latwy                           Sredni                              Trudny", czcionka, 24);
            tryby.setPosition(200.f, 465.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(tryby));

            // Wyświetlanie opcji która została wybrana klikając przycisk:
            if(poziomTrudnosci==1)
            {
                sf::Text poziom(L"Latwy", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==2)
            {
                sf::Text poziom(L"Sredni", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==3)
            {
                sf::Text poziom(L"Trudny", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(make_unique<sf::Text>(poziom));
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
                slider.setPosition(mouseX, pozycjaSuwakY);
                pozycjaSuwakX=mouseX;
            }
            wszystkieKsztalty.emplace_back(make_unique<sf::RectangleShape>(slider));

            // Tworzenie kształtów, tekstów i dodawanie ich do wektora c.d.:
            sf::Text ilosc(to_string(value), czcionka, 30);
            ilosc.setPosition(460.f, 230.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(ilosc));

            sf::Text graj(L"  GRAJ", czcionka, 45);
            graj.setPosition(570.f, 600.f);
            graj.setFillColor(sf::Color::Yellow);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(graj));

            // Wyswietlanie wszystkiego z wektora:
            window.clear(sf::Color::Black);
            for(const auto &s : wszystkieKsztalty) {
                window.draw(*s);
            }

            window.display();


        }

        if(etapGry==2)
        {
            for(const auto &s : sprajtyMain) {
                window.draw(*s);
            }

            int odswiezanie = (400/poziomTrudnosci);
            // generowanie nowego bloczka, ruch bloczka, dodanie go do Macierzy, gdy sie zatrzyma
            sf::Time czas = clock.getElapsedTime();
            if(czas.asMilliseconds()>(odswiezanie) && przegrany==false && wygrany==false)
            {
                clock.restart();

                if(bloczek.wypelnijMacierz(macierz, COLUMNS, ROWS, Macierz))
                {
                    // Dodaje do zmiennej Macierz nowe dwa elementy ktore sa efektem bloczka, ktory dopiero co spaadl
                    Macierz = dodaj_macierze(Macierz, macierz);
                    bloczek.usun(macierz, COLUMNS, ROWS);
                    Macierz = usuwanieBloczkow(Macierz, COLUMNS, ROWS, liczbaZbic);
                    Macierz = usuwanieBloczkow(Macierz, COLUMNS, ROWS, liczbaZbic);
                    Macierz = opadanieBloczkow(Macierz, ROWS, COLUMNS);
                    liczbaProb++;
                    // Ustawia wszystkie parametry domyslnie
                    bloczek.nowy();
                }
            }

            // usuwanie bloczkow jesli spelnione sa warunki


            // itercja kolorujace plansze
            for (int i=0; i< COLUMNS; i++)
            {
                for(int j=0; j < ROWS; j++)
                {
                    if(macierz[i][j]==0)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(bloczekTexture);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    // czesc iteracji kolorujaca pola ruchu bloczka na bazie macierzy
                    if(macierz[i][j]==1)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_fioletowa);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    if(macierz[i][j]==2)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_morska);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    if(macierz[i][j]==3)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_zolta);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                }
            }

            sf::Time elapsed = clock2.getElapsedTime();
            //iteracja kolorujaca wszystkie pola będące w Macierzy
            for (int i=0; i< COLUMNS; i++)
            {
                for(int j=0; j < ROWS; j++)
                {
                    if(Macierz[i][j]==1)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_fioletowa);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    if(Macierz[i][j]==2)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_morska);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    if(Macierz[i][j]==3)
                    {
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTexture(pigula_zolta);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    else if (Macierz[i][j] == 101)
                    {
                        // Przełączanie między teksturami co określony czas
                        if (elapsed.asSeconds() >= animationTime)
                        {
                            Sprajty[i][j].setTexture(potworFioletowyTextures[1]);
                            if (elapsed.asSeconds() >= 2*animationTime)
                                clock2.restart();
                        }
                        else
                        {
                            Sprajty[i][j].setTexture(potworFioletowyTextures[0]);
                        }
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    else if (Macierz[i][j] == 102)
                    {
                        // Przełączanie między teksturami co określony czas
                        if (elapsed.asSeconds() >= animationTime)
                        {
                            Sprajty[i][j].setTexture(potworMorskiTextures[1]);
                            if (elapsed.asSeconds() >= 2*animationTime)
                                clock2.restart();
                        }
                        else
                        {
                            Sprajty[i][j].setTexture(potworMorskiTextures[0]);
                        }
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                    else if (Macierz[i][j] == 103)
                    {
                        // Przełączanie między teksturami co określony czas
                        if (elapsed.asSeconds() >= animationTime)
                        {
                            Sprajty[i][j].setTexture(potworZoltyTextures[1]);
                            if (elapsed.asSeconds() >= 2*animationTime)
                                clock2.restart();
                        }
                        else
                        {
                            Sprajty[i][j].setTexture(potworZoltyTextures[0]);
                        }
                        Sprajty[i][j].setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j * 4 + pozY);
                        Sprajty[i][j].setTextureRect(sf::IntRect(0, 0, 6, 6));
                        window.draw(Sprajty[i][j]);
                    }
                }
            }


            //            // itercja kolorujaca plansze
            //            for (int i=0; i< COLUMNS; i++)
            //            {
            //                for(int j=0; j < ROWS; j++)
            //                {
            //                    // czesc iteracji kolorujaca nieaktywne szare kartki na bazie macierzy
            //                    if(macierz[i][j]==0)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(100, 100, 100));
            //                        window.draw(rectangle);
            //                    }
            //                    // czesc iteracji kolorujaca pola ruchu bloczka na bazie macierzy
            //                    if(macierz[i][j]==1)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(0, 0, 200));
            //                        window.draw(rectangle);
            //                    }
            //                    if(macierz[i][j]==2)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(0, 200, 0));
            //                        window.draw(rectangle);
            //                    }
            //                    if(macierz[i][j]==3)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(200, 0, 0));
            //                        window.draw(rectangle);
            //                    }
            //                }
            //            }

            //            //iteracja kolorujaca wszystkie pola będące w Macierzy
            //            for (int i=0; i< COLUMNS; i++)
            //            {
            //                for(int j=0; j < ROWS; j++)
            //                {
            //                    if(Macierz[i][j]==1 || Macierz[i][j]==101)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(0, 0, 200));
            //                        window.draw(rectangle);
            //                    }
            //                    if(Macierz[i][j]==2 || Macierz[i][j]==102)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(0, 200, 0));
            //                        window.draw(rectangle);
            //                    }
            //                    if(Macierz[i][j]==3 || Macierz[i][j]==103)
            //                    {
            //                        rectangle.setPosition(CELL_SIZE * i * 4 + pozX, CELL_SIZE * j *4+pozY);
            //                        rectangle.setFillColor(sf::Color(200, 0, 0));
            //                        window.draw(rectangle);
            //                    }
            //                }
            //            }







            liczbaPotworkow=licznik(Macierz, COLUMNS, ROWS);
            sf::Text potw(L"Zostalo ci jeszcze", czcionka, 30);
            potw.setPosition(580.f, 230.f);
            window.draw(potw);
            sf::Text licz(to_string(liczbaPotworkow), czcionka, 35);
            licz.setPosition(700.f, 280.f);
            window.draw(licz);
            sf::Text pro(L"Proba", czcionka, 30);
            pro.setPosition(580.f, 300.f);
            window.draw(pro);
            sf::Text pr(to_string(liczbaProb), czcionka, 35);
            pr.setPosition(700.f, 330.f);
            window.draw(pr);
            wygrany=czyWygrany(COLUMNS, ROWS, Macierz);
            przegrany=czyPrzegrany(Macierz);

            if (przegrany==false && wygrany==false)
                clock1.restart();

            if(wygrany)
            {
                wynikKoncowy=((100*value)+(10*liczbaZbic)+liczbaProb)*poziomTrudnosci;
                sf::Time czas1 = clock1.getElapsedTime();
                if(czas1.asMilliseconds()>(1000))
                {

                    window.clear(sf::Color::Black);

                    window.draw(tloKonSpriteW);

                    sf::Text wygr(L"WYGRALES", czcionka, 30);
                    wygr.setPosition(210.f, 230.f);
                    window.draw(wygr);

                    sf::Text wyn(L"Wynik", czcionka, 30);
                    wyn.setPosition(220.f, 290.f);
                    window.draw(wyn);
                    sf::Text pr(to_string(wynikKoncowy), czcionka, 35);
                    pr.setPosition(240.f, 315.f);
                    window.draw(pr);

                    if(zapis){
                        std::ifstream plik("textury/liczby.txt");
                        if (plik.is_open()) {
                            int liczba;
                            liczby.empty();

                            while (plik >> liczba) {
                                liczby.emplace_back(liczba);
                            }

                            plik.close();
                        } else {
                            std::cout << "Nie można otworzyć pliku." << std::endl;
                            return 1;
                        }

                        // Wprowadzenie nowej liczby
                        int nowaLiczba= wynikKoncowy;

                        bool jestMniejszaLubRowna = false;
                        for (int liczba : liczby) {
                            if (nowaLiczba >= liczba) {
                                jestMniejszaLubRowna = true;
                                break;
                            }
                        }

                        // Aktualizacja pliku, jeśli nowa liczba jest większa/równa
                        if (jestMniejszaLubRowna) {
                            liczby.emplace_back(nowaLiczba);

                            // Sortowanie w kolejności malejącej
                            std::sort(liczby.rbegin(), liczby.rend());

                            // Usunięcie nadmiarowych liczb
                            while (liczby.size() > 3) {
                                liczby.pop_back();
                            }

                            std::ofstream plikWyjsciowy("textury/liczby.txt");
                            if (plikWyjsciowy.is_open()) {
                                for (int liczba : liczby) {
                                    plikWyjsciowy << liczba << std::endl;
                                }
                                plikWyjsciowy.close();
                            }
                        }
                        zapis=false;
                    }

                    sf::Text naj(L"Najlepsze wyniki", czcionka, 30);
                    naj.setPosition(220.f, 390.f);
                    window.draw(naj);
                    int a=liczby[0];
                    int b=liczby[1];
                    int c=liczby[2];
                    std::wstring aText = L"1   " + std::to_wstring(a);
                    std::wstring bText = L"2   " + std::to_wstring(b);
                    std::wstring cText = L"3   " + std::to_wstring(c);

                    sf::Text w(aText, czcionka, 35);
                    w.setPosition(240.f, 420.f);
                    window.draw(w);
                    sf::Text wa(bText, czcionka, 35);
                    wa.setPosition(240.f, 450.f);
                    window.draw(wa);
                    sf::Text wr(cText, czcionka, 35);
                    wr.setPosition(240.f, 480.f);
                    window.draw(wr);

                    for(const auto &s : sprajtyKon) {
                        window.draw(*s);
                    }
                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 550.0);
                    prostokat1.setFillColor(sf::Color::Green);
                    window.draw(prostokat1);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 550.0);
                    prostokat2.setFillColor(sf::Color::Blue);
                    window.draw(prostokat2);

                    sf::Text re(L"Sprobuj ponownie", czcionka, 20);
                    re.setPosition(210.f, 555.f);
                    window.draw(re);

                    sf::Text wy(L"Wyjdz", czcionka, 20);
                    wy.setPosition(430.f, 555.f);
                    window.draw(wy);
                }
            }
            if(przegrany)
            {
                sf::Time czas1 = clock1.getElapsedTime();
                if(czas1.asMilliseconds()>(1000))
                {
                    window.clear(sf::Color::Black);
                    window.draw(tloKonSpriteL);
                    sf::Text prz(L"PRZEGRALES", czcionka, 30);
                    prz.setPosition(210.f, 230.f);
                    window.draw(prz);

                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 550.0);
                    prostokat1.setFillColor(sf::Color::Green);
                    window.draw(prostokat1);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 550.0);
                    prostokat2.setFillColor(sf::Color::Blue);
                    window.draw(prostokat2);

                    sf::Text re(L"Sprobuj ponownie", czcionka, 20);
                    re.setPosition(210.f, 555.f);
                    window.draw(re);

                    sf::Text wy(L"Wyjdz", czcionka, 20);
                    wy.setPosition(430.f, 555.f);
                    window.draw(wy);

                    sf::Text zb(L"Zostalo ci do zbicia jeszcze ", czcionka, 30);
                    zb.setPosition(220.f, 310.f);
                    window.draw(zb);

                    sf::Text licz(to_string(liczbaPotworkow), czcionka, 35);
                    licz.setPosition(380.f, 350.f);
                    window.draw(licz);
                }
            }

            window.display();
        }


    }

    return 0;
}


Matrix dodaj_macierze(const Matrix& macierz1, const Matrix& macierz2) {
    // Sprawdzenie wymiarów macierzy
    if (macierz1.size() != macierz2.size() || macierz1[0].size() != macierz2[0].size()) {
        throw invalid_argument("Macierze musza miec takie same wymiary.");
    }

    Matrix suma_macierzy;
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


Matrix usuwanieBloczkow(Matrix& Macierz, int rows, int cols, int &liczbaZbic)
{
    Matrix tempMacierz = Macierz;

    // Sprawdź wiersze
    for (auto& row : tempMacierz) {
        int licznik = 1;
        int poprzedniNumer = row[0];

        if(poprzedniNumer<100)
            poprzedniNumer=poprzedniNumer+100;
        for (size_t i = 1; i < row.size(); i++) {
            if (row[i] == poprzedniNumer || row[i]+100==poprzedniNumer) {
                licznik++;
                if (licznik >= 4) {
                    if(poprzedniNumer!=0 && poprzedniNumer!=100)
                    {
                        liczbaZbic++;
                    }
                    for (size_t j = i - licznik + 1; j <= i; j++) {
                        Macierz[&row - &tempMacierz[0]][j] = 0;
                    }
                }
            } else {
                licznik = 1;
                poprzedniNumer = row[i];
                if(poprzedniNumer<100)
                    poprzedniNumer=poprzedniNumer+100;
            }
        }
    }

    // Sprawdź kolumny
    for (int j = 0; j < cols; j++) {
        int licznik = 1;
        int poprzedniNumer = tempMacierz[0][j];

        if(poprzedniNumer<100)
            poprzedniNumer=poprzedniNumer+100;
        for (int i = 1; i < rows; i++) {
            if (tempMacierz[i][j] == poprzedniNumer || tempMacierz[i][j]+100 == poprzedniNumer) {
                licznik++;
                if (licznik >= 4) {
                    if(poprzedniNumer!=0 && poprzedniNumer!=100)
                    {
                        liczbaZbic++;
                    }
                    for (int k = i - licznik + 1; k <= i; k++) {
                        Macierz[k][j] = 0;
                    }
                }
            } else {
                licznik = 1;
                poprzedniNumer = tempMacierz[i][j];
                if(poprzedniNumer<100)
                    poprzedniNumer=poprzedniNumer+100;
            }
        }
    }
    return Macierz;
}
bool czyWygrany(int COLUMNS, int ROWS, Matrix Macierz)
{
    for (int i=0; i< COLUMNS; i++)
    {
        for(int j=0; j < ROWS; j++)
        {
            if(Macierz[i][j]>100)
            {
                return false;
            }
        }
    }
    return true;
}
bool czyPrzegrany(Matrix Macierz)
{
    if(Macierz[3][0]!=0)
        return true;
    else
        return false;
}
int licznik(Matrix Macierz, int COLUMNS, int ROWS)
{
    int a=0;
    for (int i=0; i< COLUMNS; i++)
    {
        for(int j=0; j < ROWS; j++)
        {
            if(Macierz[i][j]>100)
            {
                a++;
            }
        }
    }
    return a;
}


void wczytajProstokaty(vector<unique_ptr<sf::Drawable>> &ksztalty)
{
    sf::RectangleShape prostokat1(sf::Vector2f(145.0, 45.0));
    prostokat1.setPosition(200.0, 460.0);
    prostokat1.setFillColor(sf::Color::Green);
    ksztalty.emplace_back(make_unique<sf::RectangleShape>(prostokat1));

    sf::RectangleShape prostokat2(sf::Vector2f(170.0, 45.0));
    prostokat2.setPosition(345.0, 460.0);
    prostokat2.setFillColor(sf::Color::Blue);
    ksztalty.emplace_back(make_unique<sf::RectangleShape>(prostokat2));

    sf::RectangleShape prostokat3(sf::Vector2f(170.0, 45.0));
    prostokat3.setPosition(515.0, 460.0);
    prostokat3.setFillColor(sf::Color::Red);
    ksztalty.emplace_back(make_unique<sf::RectangleShape>(prostokat3));

    sf::RectangleShape track(sf::Vector2f(400, 10));
    track.setFillColor(sf::Color::White);
    track.setPosition(200, 300);
    ksztalty.emplace_back(make_unique<sf::RectangleShape>(track));

    sf::RectangleShape przyciskGraj(sf::Vector2f(170, 80));
    przyciskGraj.setFillColor(sf::Color::Red);
    przyciskGraj.setPosition(550, 590);
    ksztalty.emplace_back(make_unique<sf::RectangleShape>(przyciskGraj));
}

Matrix opadanieBloczkow(Matrix& macierz, int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        for (int j = rows - 1; j > 0; j--) {

            int k=0;
            if(i==cols-1)
            {
                if(macierz[i][j-1]<100 && macierz[i][j-1]>0  && macierz[i-1][j-1]==0){
                    if (macierz[i][j] == 0) {
                        k=j;
                        do
                        {
                            macierz[i][k] = macierz[i][k-1];
                            macierz[i][k-1] = 0;
                            k++;
                        } while (k<rows-1 && macierz[i][k]==0);

                    }
                }
            }
            if(i==0)
            {
                if(macierz[i][j-1]<100 && macierz[i][j-1]>0  && macierz[i+1][j-1]==0){
                    if (macierz[i][j] == 0) {
                        k=j;
                        do
                        {
                            macierz[i][k] = macierz[i][k-1];
                            macierz[i][k-1] = 0;
                            k++;
                        } while (k<rows-1 && macierz[i][k]==0);

                    }
                }
            }
            if(i<cols-1)
            {
                if(macierz[i][j-1]<100 && macierz[i][j-1]>0 && macierz[i+1][j-1]==0 && macierz[i-1][j-1]==0){
                    if (macierz[i][j] == 0) {
                        k=j;
                        do
                        {
                            macierz[i][k] = macierz[i][k-1];
                            macierz[i][k-1] = 0;
                            k++;
                        } while (k<rows && macierz[i][k]==0);

                    }
                }
            }

        }
    }
    return macierz;
}

std::vector<std::vector<sf::Sprite>> stworzSprajty(int cols, int rows)
{
    std::vector<std::vector<sf::Sprite>> sprajty;

    for (int i = 0; i < cols; ++i)
    {
        std::vector<sf::Sprite> sprajtyVector;

        for (int j = 0; j < rows; ++j)
        {
            sf::Sprite sprite;

            sprite.setScale(sf::Vector2f(6,6));

            sprajtyVector.push_back(sprite);
        }

        sprajty.push_back(sprajtyVector);
    }

    return sprajty;
}

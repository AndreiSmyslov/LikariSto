#include "potwory.h"
#include "bloczek.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix dodaj_macierze(const Matrix& macierz1, const Matrix& macierz2);
Matrix usuwanieBloczkow(Matrix& Macierz, int rows, int cols);
bool czyWygrany(int COLUMNS, int ROWS, Matrix Macierz);
bool czyPrzegrany(Matrix Macierz);
void wczytajProstokaty(vector<unique_ptr<sf::Drawable>> &ksztalty);
int licznik(Matrix Macierz, int, int);
Matrix opadanieBloczkow(Matrix& macierz, set<int> stop, int rows, int cols);


int main() {


    sf::Texture pigula_zolta;
    if (!pigula_zolta.loadFromFile("textury/pigula_zolta_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 1." << std::endl;
        return -1;
    }

    sf::Texture pigula_fioletowa;
    if (!pigula_zolta.loadFromFile("textury/pigula_fioletowa_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 2." << std::endl;
        return -1;
    }

    sf::Texture pigula_morska;
    if (!pigula_zolta.loadFromFile("textury/pigula_morski_texture.png")) {
        std::cerr << "Błąd podczas wczytywania tekstury 3." << std::endl;
        return -1;
    }

    sf::Sprite sprite;
    sprite.setScale(5.4,5.4);



    srand(time(NULL));

    // startowe informacje o planszy
    int CELL_SIZE = 9;
    int COLUMNS = 8;
    int SCREEN_RESIZE = 4;
    int ROWS = 16;

    // przesuniecie planszy
    int pozY = 240;
    int pozX = 300;

    // set uzyty przy okreslaniu ktore bloczki maja nie spadac w glownej petli
    set<int> spadanieStop = {101,102,103};

    // W zmiennej czcionka mamy czcionke, jezeli chcesz polskie znaki to przed tekstem daj 'L'
    // np. sf::Text text(L"śćżźą", font, 24);
    sf::Font czcionka;
    if (!czcionka.loadFromFile("arial.ttf"))
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

    bool wygrany=false;
    bool przegrany=false;

    Bloczek bloczek;

    // macierz zawiera w sobie pozycje ruchu tymczasowego bloczka
    Matrix macierz(COLUMNS, vector<int>(ROWS));
    // Macierz zawiera w sobie pozycje stałych obiektów
    Matrix Macierz(COLUMNS, vector<int>(ROWS));

    // tworzenie okienka
    sf::RenderWindow window(sf::VideoMode(900, 900), "Likaristo", sf::Style::Close);

    // rectangle kwadratem będącym kratka na plaszy
    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE*SCREEN_RESIZE-1, CELL_SIZE*SCREEN_RESIZE-1));


    sf::Texture tloTexture;
    if (!tloTexture.loadFromFile("tlo_main_gra_texture.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    tloTexture.setRepeated(true);
    sf::Sprite tloSprite;
    tloSprite.setTexture(tloTexture);
    tloSprite.setTextureRect(sf::IntRect(0, 0, 900, 900));




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
                }

                if (przegrany || wygrany) {
                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 460.0);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 460.0);

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

            sf::Text tryb(L"Ilość Wirusów:", czcionka, 30);
            tryb.setPosition(210.f, 230.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(tryb));

            sf::Text szybkosc(L"Tryb gry:", czcionka, 30);
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

            sf::Text tryby(L"1. Tryb łatwy  2. Tryb średni  3. Tryb trudny", czcionka, 24);
            tryby.setPosition(200.f, 465.f);
            wszystkieKsztalty.emplace_back(make_unique<sf::Text>(tryby));

            // Wyświetlanie opcji która została wybrana klikając przycisk:
            if(poziomTrudnosci==1)
            {
                sf::Text poziom(L"Łatwy", czcionka, 25);
                poziom.setPosition(440.f, 390.f);
                wszystkieKsztalty.emplace_back(make_unique<sf::Text>(poziom));
            }

            if(poziomTrudnosci==2)
            {
                sf::Text poziom(L"Średni", czcionka, 25);
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

            sf::Text graj(L"GRAJ", czcionka, 45);
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
            window.draw(tloSprite);

            // generowanie nowego bloczka, ruch bloczka, dodanie go do Macierzy, gdy sie zatrzyma
            sf::Time czas = clock.getElapsedTime();
            if(czas.asMilliseconds()>(300) && przegrany==false && wygrany==false)
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


            Macierz = opadanieBloczkow(Macierz, spadanieStop, ROWS, COLUMNS);

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

            liczbaPotworkow=licznik(Macierz, COLUMNS, ROWS);
            sf::Text potw(L"Zostało ci jeszcze:", czcionka, 30);
            potw.setPosition(620.f, 230.f);
            window.draw(potw);
            sf::Text licz(to_string(liczbaPotworkow), czcionka, 35);
            licz.setPosition(660.f, 280.f);
            window.draw(licz);
            wygrany=czyWygrany(COLUMNS, ROWS, Macierz);
            przegrany=czyPrzegrany(Macierz);

            if (przegrany==false && wygrany==false)
                clock1.restart();

            if(wygrany)
            {
                sf::Time czas1 = clock1.getElapsedTime();
                if(czas1.asMilliseconds()>(1000))
                {
                    window.clear(sf::Color::Black);
                    sf::Text wygr(L"WYGRAŁEŚ:", czcionka, 30);
                    wygr.setPosition(210.f, 230.f);
                    window.draw(wygr);

                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 460.0);
                    prostokat1.setFillColor(sf::Color::Green);
                    window.draw(prostokat1);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 460.0);
                    prostokat2.setFillColor(sf::Color::Blue);
                    window.draw(prostokat2);

                    sf::Text re(L"Spróbuj ponownie:", czcionka, 20);
                    re.setPosition(210.f, 465.f);
                    window.draw(re);

                    sf::Text wy(L"Wyjdź:", czcionka, 20);
                    wy.setPosition(430.f, 465.f);
                    window.draw(wy);
                }
            }
            if(przegrany)
            {
                sf::Time czas1 = clock1.getElapsedTime();
                if(czas1.asMilliseconds()>(1000))
                {
                    window.clear(sf::Color::Black);
                    sf::Text prz(L"PRZEGRAŁEŚ:", czcionka, 30);
                    prz.setPosition(210.f, 230.f);
                    window.draw(prz);

                    sf::RectangleShape prostokat1(sf::Vector2f(220.0, 60.0));
                    prostokat1.setPosition(200.0, 460.0);
                    prostokat1.setFillColor(sf::Color::Green);
                    window.draw(prostokat1);

                    sf::RectangleShape prostokat2(sf::Vector2f(220.0, 60.0));
                    prostokat2.setPosition(420.0, 460.0);
                    prostokat2.setFillColor(sf::Color::Blue);
                    window.draw(prostokat2);

                    sf::Text re(L"Spróbuj ponownie:", czcionka, 20);
                    re.setPosition(210.f, 465.f);
                    window.draw(re);

                    sf::Text wy(L"Wyjdź:", czcionka, 20);
                    wy.setPosition(430.f, 465.f);
                    window.draw(wy);

                    sf::Text zb(L"Zostało ci do zbicia jeszcze: ", czcionka, 30);
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

Matrix usuwanieBloczkow(Matrix& Macierz, int rows, int cols)
{
    Matrix tempMacierz = Macierz;

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


void wczytajProstokaty(vector<unique_ptr<sf::Drawable> > &ksztalty)
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

Matrix opadanieBloczkow(Matrix& macierz, set<int> stop, int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        for (int j = rows - 1; j > 0; j--) {
            if (macierz[i][j] == 0 && stop.find(macierz[i][j-1]) != stop.end()) {
                macierz[i][j] = macierz[i ][j-1];
                macierz[i][j-1] = 0;
            }
        }
    }
    return macierz;
}

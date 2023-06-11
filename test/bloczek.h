#ifndef BLOCZEK_H
#define BLOCZEK_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "SFML/System/Vector2.hpp"
#include <vector>
class Bloczek
{
public:
    Bloczek();
    bool wypelnijMacierz(std::vector<std::vector<int>> &macierz, int COLUMNS, int ROWS, std::vector<std::vector<int>> Macierz);
    void przesunPrawo(std::vector<std::vector<int>> Macierz);
    void przesunLewo(std::vector<std::vector<int> > Macierz);
    void nowy();
    void obrocLewo();
    void obrocPrawo();
private:
    std::vector<std::vector<int>> wektor;
    int pozycja =3;
    bool przesuwanie=true;
    int  spadanie=0;
    int numerTekstury1;
    int numerTekstury2;
    int sposobSpadania=1;

};

#endif // BLOCZEK_H

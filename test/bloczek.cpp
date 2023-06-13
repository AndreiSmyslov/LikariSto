#include "bloczek.h"
#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


Bloczek::Bloczek()
{
    numerTekstury1=rand()%3+1;
    numerTekstury2=rand()%3+1;
}

bool Bloczek::wypelnijMacierz(std::vector<std::vector<int>> &macierz, int COLUMNS, int ROWS, std::vector<std::vector<int>> Macierz)
{
    // Sprawdza czy pauza jest true, jezeli tak, to odejmuje od zmiennej spadanie, tak że spadanie ciagle jest takie samo (na koncu tej funkcji spadanie wzrasta o jeden)

    // Spadanie i pozycja to w pewien sposob takie wspolrzedne w macierzy, ktore pomagaja sie w niej odnalezc, sa wspolrzednymi konkretnego kwadratu lub elementu w macierzy
    if(pauza)
    {
        spadanie--;
        przesuwanie=false;
    }
    else
        przesuwanie = true;
    for (int i=0; i< COLUMNS; i++)
    {
        for(int j=0; j < ROWS; j++)
        {
            // Sposob spadania to zmienna mowiaca czy obiekt zostal obrocony i czy jest normalnie czyt na grzbiecie
            if(sposobSpadania==1)
            {
                if((i == pozycja && j==spadanie))
                {
                    macierz[i][j]=numerTekstury1;
                    if(Macierz[i][j+1]!=0 || Macierz[i+1][j+1]!=0)
                    {
                        macierz[i+1][j-1]=0;
                        if(i!=6)
                            macierz[i+2][j-1]=0;
                        macierz[i+1][j]=numerTekstury2;
                        przesuwanie=false;
                        return true;
                    }
                }
                else if((i == pozycja+1 && j==spadanie))
                {
                    macierz[i][j]=numerTekstury2;
                }
                else
                {
                    macierz[i][j]=0;
                }
                if(spadanie>15)
                {
                    spadanie=15;
                    przesuwanie=false;
                    return true;
                }
            }
            if(sposobSpadania==2)
            {
                if((i == pozycja && j==spadanie))
                {
                    macierz[i][j]=numerTekstury1;
                    if(Macierz[i][j+2]!=0)
                    {
                        macierz[i+2][j+1]=0;
                        macierz[i+1][j]=0;
                        macierz[i+1][j+1]=0;
                        macierz[i][j+1]=numerTekstury2;
                        przesuwanie=false;
                        return true;
                    }
                }
                else if((i == pozycja && j==spadanie+1))
                {
                    macierz[i][j]=numerTekstury2;
                }
                else
                {
                    macierz[i][j]=0;
                }
                if(spadanie>14)
                {
                    spadanie=14;
                    przesuwanie=false;
                    return true;
                }
            }
            if(sposobSpadania==3)
            {
                if((i == pozycja && j==spadanie))
                {
                    macierz[i][j]=numerTekstury2;
                    if(Macierz[i][j+1]!=0 || Macierz[i+1][j+1]!=0)
                    {
                        macierz[i+1][j-1]=0;
                        if(i!=6)
                            macierz[i+2][j-1]=0;
                        macierz[i+1][j]=numerTekstury1;
                        przesuwanie=false;
                        return true;
                    }
                }
                else if((i == pozycja+1 && j==spadanie))
                {
                    macierz[i][j]=numerTekstury1;
                }
                else
                {
                    macierz[i][j]=0;
                }
                if(spadanie>15)
                {
                    spadanie=15;
                    przesuwanie=false;
                    return true;
                }
            }
            if(sposobSpadania==4)
            {
                if((i == pozycja && j==spadanie))
                {
                    macierz[i][j]=numerTekstury2;
                    if(Macierz[i][j+2]!=0)
                    {
                        //macierz[i][j-1]=0;
                        macierz[i+2][j+1]=0;
                        macierz[i+1][j]=0;
                        macierz[i+1][j+1]=0;
                        macierz[i+1][j+2]=0;

                        macierz[i][j+1]=numerTekstury1;
                        przesuwanie=false;
                        return true;
                    }
                }
                else if((i == pozycja && j==spadanie+1))
                {
                    macierz[i][j]=numerTekstury1;
                }
                else
                {
                    macierz[i][j]=0;
                }
                if(spadanie>14)
                {
                    spadanie=14;
                    przesuwanie=false;
                    return true;
                }
            }
        }
    }
    spadanie++;
    return false;
}

void Bloczek::przesunPrawo(std::vector<std::vector<int> > Macierz)
{
    if(sposobSpadania==2 && pozycja==6 && (przesuwanie==true) && Macierz[7][spadanie]==0)
        pozycja++;
    if((pozycja<6) && (przesuwanie==true) && Macierz[pozycja+2][spadanie]==0)
        pozycja++;
}

void Bloczek::przesunLewo(std::vector<std::vector<int> > Macierz)
{
    if(sposobSpadania==4 && pozycja==0 && (przesuwanie==true) && Macierz[0][spadanie]==0)
        pozycja--;

    if(pozycja>0 && przesuwanie==true && Macierz[pozycja-1][spadanie]==0)
        pozycja--;
}

void Bloczek::nowy()
{
    sposobSpadania=1;
    pozycja =3;
    przesuwanie=true;
    spadanie=0;
    numerTekstury1=rand()%3+1;
    numerTekstury2=rand()%3+1;
}

void Bloczek::obrocLewo(std::vector<std::vector<int> > Macierz)
{
    if(pozycja>0 && Macierz[pozycja][spadanie-1]==0)
    {
        sposobSpadania--;
    }
    if(sposobSpadania==0)
        sposobSpadania=4;
}
void Bloczek::obrocPrawo(std::vector<std::vector<int> > Macierz)
{
    if((sposobSpadania==2 || sposobSpadania==4) && pozycja<6 && Macierz[pozycja+1][spadanie+1]==0)
    {
        sposobSpadania++;
    }
    if(sposobSpadania==1 && pozycja<7 && Macierz[pozycja][spadanie+1]==0)
    {
        sposobSpadania++;
        pozycja++;
    }
    if(sposobSpadania==3 && pozycja<7 && Macierz[pozycja][spadanie+1]==0)
    {
        sposobSpadania++;
        pozycja--;
    }
    if(sposobSpadania==5)
        sposobSpadania=1;
}

void Bloczek::Pauza()
{
    if (pauza==true)
        pauza=false;
    else
        pauza=true;
}

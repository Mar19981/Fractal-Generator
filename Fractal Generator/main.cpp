/*###################################################################
TEMAT: GENERATOR ZBIORU MANDELBROTA I ZBIORU JULII;
OPIS: Funkcje generujace obraz zbioru Mandelbrota oraz zbioru Julii
o zadanych parametrach;
19.01.2021 r., 2020/2021, Semestr V, Marcin Nastala
WERSJA: 1.0.2
Zmiany: 1.0.2 - Dodanie generacji zbioru Julii
1.0.1 - Dodanie generacji zbioru Mandelbrota
1.0.0 - Utworzenie i implementacja zachowania GUI
####################################################################
PLIK GLOWNY PROGRAMU
*/

#include "fractalgenerator.h"
#include <QtWidgets/QApplication>

/*
Funkcja glowna programu
Argumenty: argc - ilosc parametrow wywolania, argv - tablica parametrow wywolania
Zwracana wartosc - kod wywolania aplikacji
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //Tworzenie aplikacji
    FractalGenerator w; //Tworzenie glownego okna
    w.show(); //Wyswietlenie okna
    return a.exec(); //Wywolanie aplikacji
}

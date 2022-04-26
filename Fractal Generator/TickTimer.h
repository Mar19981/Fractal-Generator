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
PLIK NAGLOWKOWY KLASY TickTimer
*/

#pragma once
#include "Timer.h"
#include <Windows.h>

//Klasa dziedziczaca po Timer, odmierzajaca czas w taktach procesora
class TickTimer :
    public Timer
{
    long long unsigned int init, //Wartosc na poczatku pomiaru
        end; //Wartosc na koncu pomiaru
public:
    /*Metoda rozpoczynajaca pomiar czasu
    * Parametry: brak
    * Zwracana wartosc: brak
    */
    void start();
    /*Metoda konczaca pomiar czasu
    * Parametry: brak
    * Zwracana wartosc: odmierzony czas
    */
    std::string stop();
};


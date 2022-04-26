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
PLIK NAGLOWKOWY SZABLONU KLASY ChronoTimer
*/
#pragma once
#include "Timer.h"
#include <chrono>
//Klasa ChronoTimer, dziedziczaca po Timer. Odmierza czas w zadanej jednostce literalow biblioteki std::chrono
template <typename T>
class ChronoTimer :
    public Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> init, //Wartosc na poczatku pomiaru
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

template<typename T>
inline void ChronoTimer<T>::start()
{
    init = std::chrono::high_resolution_clock::now(); //Pobierz czas i zapisz do skladnika init
}

template<typename T>
inline std::string ChronoTimer<T>::stop()
{
    end = std::chrono::high_resolution_clock::now(); //Pobierz czas i zapisz do skladnika end
    T duration = end - init; //Odmierz czas miedzy poczatkiem a koncem pomiaru
    return std::to_string(duration.count()); //Skonwertuj go na string i zwroc
}

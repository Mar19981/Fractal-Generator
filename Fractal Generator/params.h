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
PLIK NAGLOWKOWY STRUKTUR Z PARAMETRAMI
*/

#pragma once

#include <iostream>

//Struktura parametrow zbioru Mandelbrota
struct MandelbrotParams {
    uint8_t* arr; //Tablica pikseli
    double scale, //Skala fraktalu
        depth; //Limit fraktalu
    int dim, //Wymiar obrazu
        startY, //Wspolrzedna Y, od ktorej zaczyna sie petla
        size, //Rozmiar tablicy pikseli
        iterations; //Iteracje fraktalu
    //Konstruktor
    MandelbrotParams(uint8_t* a, int dm, int sy, int s, int i, double sc, double d) :
        arr(a), dim(dm), startY(sy), size(s), iterations(i), scale(sc), depth(d) {};
};
//Struktura parametrow zbioru Julii
struct JuliaParams {
    uint8_t* arr; //Tablica pikseli
    double scale, //Skala fraktalu
        depth, //Limit fraktalu
        real, //Parametr rzeczywisty
        imaginary, //Parametr urojony
        d, //Przyrost
        x, //Wspolrzedna x
        minimal; //Minimalna wspolrzedna
    int dim, //Wymiar obrazu
        startY, //Wspolrzedna Y, od ktorej zaczyna sie petla
        size, //Rozmiar tablicy pikseli
        iterations; //Iteracje fraktalu

    //Konstruktor
    JuliaParams(uint8_t* a, int dm, int sy, int s, int i, double sc, double d, double r, double im, double _d, double _x, double mini) :
        arr(a), dim(dm), startY(sy), size(s), iterations(i), scale(sc), depth(d), real(r), imaginary(im), d(_d), x(_x), minimal(mini) {};
};
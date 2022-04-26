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
PLIK NAGLOWKOWY FUNKCJI GENERUJACYCH ZBIORY W C++
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <thread>
#include <cmath>
#include <Windows.h>
#include "params.h"

/* Funkcja generujaca zbior Mandelbrota
* Parametry: params - wskaznik na strukture z parametrami zbioru
* Zwracana wartosc: brak
*/
void generateMandelbrotSet(MandelbrotParams* params);

/* Funkcja generujaca zbior Julii
* Parametry: params - wskaznik na strukture z parametrami zbioru
* Zwracana wartosc: brak
*/
void generateJuliaSet(JuliaParams* params);

/* Funkcja mapujaca wartosc z jednego przedzialu na drugi przedzial
* Parametry: value - wartosc do zmapowania
*	initMin - minimalna wartosc pierwszego przedzialu
*	initMax - maksymalna wartosc pierwszego przedzialu
*	newMin - minimalna wartosc drugiego przedzialu
*	newMax - maksymalna wartosc drugiego przedzialu
* Zwracana wartosc: odpowiednik zadanej wartosci w drugim przedziale
*/
inline double remap(double value, double initMin, double initMax, double newMin, double newMax) { 
	return  newMin + (value - initMin) * (newMax - newMin) / (initMax - initMin); 
}

#endif // !FUNCTIONS_H

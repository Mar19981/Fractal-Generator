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
PLIK NAGLOWKOWY KLASY ABSTRAKCYJNEJ TIMER
*/
#ifndef TIMER_H
#define TIMER_H

#include <string>

//Klasa abstrakcyjna licznika czasu
class Timer
{
public:
	/*Metoda abstrakcyjna start, rozpoczynajaca pomiar czasu
	* Parametry: brak
	* Zwracana wartosc: brak
	*/
	virtual void start() = 0;
	/*Metoda abstrakcyjna start, konczaca pomiar czasu
	* Parametry: brak
	* Zwracana wartosc: odmierzony czas
	*/
	virtual std::string stop() = 0;
};

#endif // !TIMER_H

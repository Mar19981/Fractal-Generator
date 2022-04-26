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
PLIK ZRODLOWY KLASY TickTimer
*/
#include "TickTimer.h"

void TickTimer::start()
{
	init = __rdtsc(); //Wpisz ilosc taktow do init
}
std::string TickTimer::stop()
{
	end = __rdtsc(); //Wpisz ilosc taktow do end
	return std::to_string(end - init); //Odejmij end od init i zwroc jako string
}

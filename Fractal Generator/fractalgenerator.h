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
PLIK NAGLOWKOWY KLASY GLOWNEJ
*/

#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include "ui_fractalgenerator.h"
#include <thread>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <exception>
#include <Windows.h>
#include "ChronoTimer.h"
#include "TickTimer.h"
#include "params.h"


typedef void (__cdecl *MANDELBROT)(MandelbrotParams*); //Alias wskaznika na funkcje, generujaca zbior Mandelbrota
typedef void (__cdecl *JULIA)(JuliaParams*); //Alias wskaznika na funkcje, generujaca zbior Mandelbrota


//Klasa okna aplikacji
class FractalGenerator : public QMainWindow
{
    Q_OBJECT

public:
    FractalGenerator(QWidget *parent = Q_NULLPTR); //Konstruktor

private:
    Ui::FractalGeneratorClass ui; //Ustawienie GUI
    QImage img; //Wygenerowany obraz
    bool generatedSet; //Zmienna wskazujaca generowany zbior (false - Mandelbrot, true - Julia)


    /*
        Metoda kopiujaca wygenerowany obraz do skladnika img
        Parametry: data - wektor podtablic z obrazem fraktalu
        Zwracana wartosc: brak
    */
    void copyFractalIntoImg(const std::vector<std::vector<uint8_t>>& data); 
    
    //Sloty do obslugi GUI

private slots:

    /*
        Metoda wybierajaca generowany zbior
        Parametry: index - indeks wybranego w comboboxie elementu
        Zwracana wartosc: brak
    */
    void changeGeneratedSet(int index);
    /*
    Metoda generujaca zbior
    Parametry: brak
    Zwracana wartosc: brak
    */
    void generate();
    /*
    Metoda zapisujaca zbior na dysku
    Parametry: brak
    Zwracana wartosc: brak
    */
    void saveImage();
};

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
PLIK ZRODLOWY KLASY GLOWNEJ
*/
#include "fractalgenerator.h"

using namespace std::chrono_literals; //Wykorzystanie literalow biblioteki chrono

FractalGenerator::FractalGenerator(QWidget *parent) //Konstruktor
    : QMainWindow(parent), generatedSet(false) {

    ui.setupUi(this); //Ustaw GUI

    this->setFixedSize(this->sizeHint()); //Ustaw staly rozmiar okna, zgodnie z podpowiedzia

    ui.threadSpin->setValue(std::thread::hardware_concurrency()); //Wpisz liczbe watkow procesora do spinnera ilosci watkow

    ui.viewport->setFixedWidth(200); //Ustaw szerokosc podgladu na 200px
    ui.viewport->setFixedHeight(100); //Ustaw wysokosc podgladu na 100px

    connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeGeneratedSet(int))); //Podepnij sygnal zmiany indexu na comboboxie do metody changeGeneratedSet
    connect(ui.generateButton, SIGNAL(clicked()), this, SLOT(generate())); //Podepnij klikniecia przycisku generacji do metody generate
    connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(saveImage())); //Podepnij klikniecia przycisku zapisania do metody saveImage
}
void FractalGenerator::changeGeneratedSet(int index) {

    generatedSet = index; //Wpisz do skladnika generatedSet wartosc indeksu
    if (index) { //Jezeli indeks nie jest zerem, to aktywuj wejscia parametrow liczby zespolonej

        ui.realSpin->setEnabled(true); //Aktywuj wejscie parametru rzeczywistego
        ui.imaginarySpin->setEnabled(true); //Aktywuj wejscie parametru urojonego
    }
    else { //W przeciwnym razie deaktywuj je

        ui.realSpin->setEnabled(false); //Deaktywuj wejscie parametru rzeczywistego
        ui.imaginarySpin->setEnabled(false); //Deaktywuj wejscie parametru urojonego
    }
}

void FractalGenerator::copyFractalIntoImg(const std::vector<std::vector<uint8_t>>& data)
{
    int size = ui.dimSpin->value(); //Pobierz wymiar

    img = QImage(size, size,QImage::Format::Format_ARGB32); //Utworz obraz

    std::vector<QRgb> pixels(size * size); //Zaalokuj wektor pikseli

    int ind{}; //Indeks piksela
    for (auto arr : data) { //Dla kazdej podtablicy
        for (auto color : arr) { //Dla kazdego koloru
            pixels.at(ind++) = qRgb(color, color, color); //Wpisz na indeks zadany indeks nowy kolor
        }
    }
    std::memmove(img.bits(), pixels.data(), size * size * sizeof(QRgb)); //Przenies do img zawartosc tablicy pikseli
    ui.viewport->setPixmap(QPixmap::fromImage(img.scaledToWidth(ui.viewport->width(), Qt::SmoothTransformation))); //Zaktualizuj okno podgladu
}

void FractalGenerator::generate()
{
    //Pobieranie wartosci z GUI
    int dimmension = ui.dimSpin->value(), //Pobierz wymiary 
        threadNum = ui.threadSpin->value(), //Pobierz liczbe watkow
        start_y = 0, //Poczatkowe y
        pixels = std::ceil(dimmension / threadNum) * dimmension; //Liczba pikseli na tablice w watku
    double scale = ui.scaleSpin->value(); //Pobierz skale
    bool libraryChoice = ui.asmRadioButton->isChecked(); //Sprawdz, ktora biblioteka zostala wybrana (true - asembler, false - c++)

    std::unique_ptr<Timer> timer; //Utworz obiekt licznika czasu
    std::vector<std::vector<uint8_t>> data(threadNum); //Zaalokuj tablice podtablic pikseli
    //Utworz odpowiedni obiekt licznika czasu, w zaleznosci od wyboru uzytkownika
    switch (ui.timeComboBox->currentIndex()) { 
        case 0: timer = std::make_unique<ChronoTimer<std::chrono::duration<double, std::milli>>>(); break; //Licznik milisekundowy
        case 1: timer = std::make_unique<ChronoTimer<std::chrono::duration<double, std::nano>>>(); break; //Licznik nanosekundowy
        case 2: timer = std::make_unique<TickTimer>(); break; //Licznik w taktach procesora
    }
    if (threadNum != 1) { //Jezeli wybrano wiecej niz 1 watek, uzupelnij tablice
        for (int i = 0; i < threadNum; i++)
        {
            if (i != threadNum - 1) //Niech prawie wszystkie elementy beda mialy rozmiar zmiennej pixels
                data.at(i).resize(pixels);
            else
                data.at(i).resize(dimmension * dimmension - i * pixels); //Ostatni element niech ma rozmiar odpowiadajacy ilosci pozostalych pikseli obrazu

        }
    }
    else 
        data.at(0).resize(dimmension * dimmension); //W przeciwnym razie zaalokuj jeden element o wymiarach obrazu

    HINSTANCE hDll{}; //Zmienna z biblioteka
    bool freeResult = false, //Rezultat zwolnienia biblioteki
        runtimeLinkSuccess = false; //Rezultat linkowania dynamicznego
    if (libraryChoice) //Jezeli zaznaczono przycisk asemblera, to zaladuj biblioteke asemblerowa
        hDll = LoadLibrary(TEXT("Asmlib.dll"));
    else
        hDll = LoadLibrary(TEXT("C++lib.dll")); //W przeciwnym razie zaladuj biblioteke c++
    try {

        if (hDll) { //Jezeli udalo sie zaladowac DLL
            if (generatedSet) { //Jezeli wybrano zbior Julii

                JULIA functionAddress = (JULIA)GetProcAddress(hDll, "generateJuliaSet"); //Sprobuj zaladowac funkcje generujaca zbior Julii

                if (functionAddress) { //Jezeli sie udalo

                    runtimeLinkSuccess = true; //Linkowanie powiodlo sie
                    double minimal = -scale * 0.5, //Oblicz minimalna wspolrzedna
                        d = scale / dimmension, //Oblicz przyrost
                        x = minimal; //Wpisz minimalna wartosc do x
                    if (threadNum > 1) //Jezeli wybrano wiecej niz 1 watek
                    {
                        std::vector<std::thread> threads; //Utworz wektor watkow
                        std::vector<std::shared_ptr<JuliaParams>> params; //Utworz wektor parametrow funkcji
                        for (int i = 0; i < threadNum; i++) { //Dla kazdego watku

                            uint8_t* ptr = data.at(i).data(); //Pozyskaj wewnetrzna tablice z wektora podtablic

                            params.emplace_back(std::make_shared<JuliaParams>(ptr, dimmension, start_y, (int)data.at(i).size(), 
                                ui.iterSpin->value(), scale, ui.limitSpin->value(), 
                                ui.realSpin->value(), ui.imaginarySpin->value(), d, x, minimal)); //Utworz nowy parametr
                            JuliaParams* paramsPtr = params.at(i).get(); //Uzyskaj wskaznik do tego parametru
                            threads.emplace_back(std::thread([=] {(functionAddress)(paramsPtr); })); //Utworz nowy watek
                            start_y += pixels / dimmension; //Zwieksz poczatkowe y
                            x += d * pixels / dimmension; //Zwieksz x
                        }
                        timer->start(); //Rozpocznij pomiar czasu
                        for (auto& thread : threads) { //Wywolaj watki
                            if (thread.joinable())
                                thread.join();
                        }
                        ui.executionTimeLabel->setText(timer->stop().c_str()); //Skoncz pomiar czasu i wypisz go do interfejsu
                    }
                    else { //W przeciwnym razie wywolaj funkcje raz

                        uint8_t* dataPtr = data.at(0).data(); //Pozyskaj wewnetrzna tablice z wektora
                        std::shared_ptr<JuliaParams> params = std::make_shared<JuliaParams>(dataPtr, dimmension, start_y, 
                            (int)data.at(0).size(), ui.iterSpin->value(), scale, ui.limitSpin->value(),
                            ui.realSpin->value(), ui.imaginarySpin->value(), d, x, minimal); //Utworz parametr
                        JuliaParams* paramsPtr = params.get(); //Pozyskaj wskaznik na parametr

                        timer->start(); //Rozpocznij pomiar czasu
                        (functionAddress)(paramsPtr); //Wywolaj funkcje
                        ui.executionTimeLabel->setText(timer->stop().c_str());  //Skoncz pomiar czasu i wypisz go do interfejsu
                    }
                        copyFractalIntoImg(data); //Skopiuj obraz do skladnika img

                }
            }
            else { //Wygeneruj zbior Mandelbrota

                MANDELBROT functionAddress = (MANDELBROT)GetProcAddress(hDll, "generateMandelbrotSet"); //Sprobuj zaladowac funkcje generujaca zbior Mandelbrota
                if (functionAddress) { //Jezeli sie udalo

                    runtimeLinkSuccess = true; //Linkowanie powiodlo sie

                    if (threadNum > 1) { //Dla wielu watkow

                        std::vector<std::thread> threads; //Utworz wektor watkow
                        std::vector<std::shared_ptr<MandelbrotParams>> params; //Utworz wektor parametrow
                        for (int i = 0; i < threadNum; i++) { //Dla kazdego watku

                            uint8_t* ptr = data.at(i).data(); //Pozyskaj wewnetrzna tablice z wektora podtablic
                            params.emplace_back(std::make_shared<MandelbrotParams>(ptr, dimmension, start_y, 
                                (int)data.at(i).size(), ui.iterSpin->value(), scale, ui.limitSpin->value() )); //Utworz parametr
                            MandelbrotParams* paramsPtr = params.at(i).get(); //Pozyskaj wskaznik na parametr
                            threads.emplace_back(std::thread([=] {(functionAddress)(paramsPtr); })); //Utworz watek
                            start_y += pixels / dimmension; //Zwieksz poczatkowe y
            	            }

                        timer->start(); //Rozpocznij pomiar czasu
                        for (auto& thread : threads) { //Wywolaj watki
                            if (thread.joinable())
                                thread.join();
                        }
                        ui.executionTimeLabel->setText(timer->stop().c_str()); //Skoncz pomiar czasu i wypisz go do interfejsu

                    }
                    else {
                        uint8_t* dataPtr = data.at(0).data(); //Pozyskaj wewnetrzna tablice z wektora podtablic
                        std::shared_ptr<MandelbrotParams> params = std::make_shared<MandelbrotParams>(dataPtr, dimmension, start_y, 
                            (int)data.at(0).size(), ui.iterSpin->value(), scale, ui.limitSpin->value()); //Utworz parametr
                        MandelbrotParams* paramsPtr = params.get(); //Pozyskaj wskaznik na parametr

                        timer->start(); //Rozpocznij pomiar czasu
                        (functionAddress)(paramsPtr); //Wywolaj funkcje
                        ui.executionTimeLabel->setText(timer->stop().c_str()); //Skoncz pomiar czasu i wypisz go do interfejsu
                    }
                    copyFractalIntoImg(data); //Skopiuj obraz do skladnika img
                }
            }
           freeResult = FreeLibrary(hDll); //Zwolnij biblioteke
        }
        if (!runtimeLinkSuccess) //Jezeli nie udalo sie zaladowac biblioteki - wyswietl komunikat
            QMessageBox::warning(this, "Nie zaladowano biblioteki", "Nie udalo sie zaladowac biblioteki!"); //Wyswietlenie ostrzezenia
    }
    catch (std::exception& e) { //Jezeli rzucono wyjatek - wyswietl komunikat

        QMessageBox::warning(this, "Blad", e.what()); //Wyswietlenie ostrzezenia
    }
}

void FractalGenerator::saveImage()
{
    if (!img.isNull()) { //Jezeli img nie jest puste

        auto fileName = QFileDialog::getSaveFileName(this, "Zapisz plik", "", "*.jpg;; *.png;; *.bmp"); //Wyswietl dialog zapisu pliku i pobierz sciezke
        if (!fileName.isEmpty()) //Jezeli nie jest pusta, zapisz plik
            img.save(fileName); //Zapis
    }
    else //W przeciwnym razie wyswietl ostrzezenie
        QMessageBox::warning(this, "Nie wygenerowano obrazu!", "Obraz nie zostal wygenerowany!"); //Wyswietlenie ostrzezenia
}

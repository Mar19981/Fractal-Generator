;####################################################################
;TEMAT: GENERATOR ZBIORU MANDELBROTA I ZBIORU JULII;
;OPIS: Funkcje generujace obraz zbioru Mandelbrota oraz zbioru Julii
;o zadanych parametrach;
;19.01.2021 r., 2020/2021, Semestr V, Marcin Nastala
;WERSJA: 1.0.2
;Zmiany: 1.0.2 - Dodanie generacji zbioru Julii
;1.0.1 - Dodanie generacji zbioru Mandelbrota
;1.0.0 - Utworzenie i implementacja zachowania GUI
;####################################################################
;PLIK Z ASEMBLEROWA IMPLEMENTACJA FRAKTALI

.data

MandelbrotStruct struct ;Struktura przechowujaca parametry zbioru Mandelbrota
	arr dq ? ;Tablica pikseli
	scale dq ? ;Skala fraktalu
	depth dq ? ;Limit fraktalu
	dim dd ? ;Wymiar obrazu
	startY dd ? ;Wspolrzedna Y, od ktorej zaczyna sie petla
	s dd ? ;Rozmiar tablicy pikseli
	iter dd ? ;Iteracje fraktalu
	MandelbrotStruct ends

JuliaStruct struct ;Struktura przechowujaca parametry zbioru Julii
	arr dq ? ;Tablica pikseli
	scale dq ? ;Skala fraktalu
	depth dq ? ;Limit fraktalu
	r dq ? ;Parametr rzeczywisty
	im dq ? ;Parametr urojony
	d dq ? ;Przyrost
	x dq ? ;Wspolrzedna x
	minimal dq ? ;Minimalna wspolrzedna
	dim dd ? ;Wymiar obrazu
	startY dd ? ;Wspolrzedna Y, od ktorej zaczyna sie petla
	s dd ? ;Rozmiar tablicy pikseli
	iter dd ? ;Iteracje fraktalu
	JuliaStruct ends

negative real8 -1.0 ;Zmienna z wartoscia -1
maxValue real8 255.0 ;Maksymalna wartosc piksela

.code

;Funkcje potrzebne do wygenerowania dll
DllEntry  proc hInstDll: dword, fdwReason: dword, lpReserved: dword
	mov rax, 1
	ret
DllEntry  endp

_DllMainCRTStartup proc
mov rax, 1
ret
_DllMainCRTStartup endp

;Funkcja generujaca zbior Mandelbrota
;Na podstawie wspolrzednych generowane sa parametry a i b fraktalu. Nastepnie na podstawie liczby iteracji okreslana jest wartosc piksela
;Parametry wejsciowe - wskaznik na strukture parametrow MandelbrotStruct
;Parametry wyjsciowe - brak
;Zmieniane flagi: EI, ZR

generateMandelbrotSet proc

;Odlozenie zawartosci rejestrow na stos
push rax
push rbx
push rdx
push r8
push r9
push r10
push r11
push r12
push r13
push rdi
push rbp

mov rbp, [rcx].MandelbrotStruct.arr ;Wpisz adres tablicy do rbp
mov r8d, [rcx].MandelbrotStruct.startY ;Wpisz startY do r8
mov r9d, [rcx].MandelbrotStruct.iter ;Wpisz ilosc iteracji do r9
mov r10d, [rcx].MandelbrotStruct.s ;Wpisz rozmiar do r10
mov r13d, [rcx].MandelbrotStruct.dim ;Wpisz wymiar do r13
movsd xmm8, [rcx].MandelbrotStruct.scale ;Wpisz skale do xmm8
movsd xmm10, [rcx].MandelbrotStruct.depth ;Wpisz limit do xmm10
movsd xmm9, negative ;Wpisz -1 do xmm9
mulsd xmm9, xmm8 ;Pomnoz skale przez -1

xor rdi, rdi ;Wyzeruj rdi
jmp start ;rozpocznij petle

yloop: ;Petla zewnetrzna

inc r8d ;Zwieksz wspolrzedna y

start: 
xor r11, r11 ;Wyzeruj wspolrzedna x

xloop: ;Petla wewnetrzna

;Oblicz wspolczynnik a liczby zespolonej

cvtsi2sd xmm2, r11d ;Przepisz wspol. x do xmm2
cvtsi2sd xmm3, r13d ;Przepisz wymiar do xmm3
movapd xmm4, xmm8 ;Przepisz -skale do xmm4
movapd xmm0, xmm9 ;Przepisz skale do xmm0
addsd xmm4, xmm4 ;Dwukrotnosc rejestru xmm4
divsd xmm4, xmm3 ;Podziel xmm4 przez wymiar 
mulsd xmm2, xmm4 ;Pomnoz wspol. x przez xmm4
addsd xmm0, xmm2 ;Dodaj do skali xmm2

;Oblicz wspolczynnik b liczby zespolonej

cvtsi2sd xmm2, r8d ;Wpisz do xmm2 wspol. y
movapd xmm4, xmm8 ;Przepisz -skale do xmm4
movapd xmm1, xmm9 ;Przepisz skale do xmm1
addsd xmm4, xmm4 ;Dwukrotnosc rejestru xmm4
divsd xmm4, xmm3  ;Podziel xmm4 przez wymiar 
mulsd xmm2, xmm4 ;Pomnoz wspol. y przez xmm4
addsd xmm1, xmm2 ;Dodaj do skali xmm2

movapd xmm2, xmm0 ;Wpisz pierwotnie obliczone a do xmm2
movapd xmm3, xmm1 ;Wpisz pierwotnie obliczone b do xmm3
xor rbx, rbx ;Wyzeruj rbx przed iteracja

iterating: ;Petla generujaca fraktal

movapd xmm4, xmm0 ;Wpisz a do xmm4
movapd xmm5, xmm1 ;Wpisz b do xmm5
mulsd xmm4, xmm4 ;Podnies a do kwadratu
mulsd xmm5, xmm5 ;Podnies b do kwadratu
subsd xmm4, xmm5 ;a^2 - b^2
addsd xmm4, xmm2 ;a^2 - b^2 + pierowtna a

movapd xmm5, xmm0 ;Wpisz a do xmm5
mulsd xmm5, xmm1 ;a * b
addsd xmm5, xmm5 ;2ab
addsd xmm5, xmm3 ;2ab + pierwotne b
movapd xmm0, xmm4 ;a = (a^2 - b^2) + pierwotne a
movapd xmm1, xmm5 ;b = 2ab + pierwotne b
mulsd xmm5, xmm5; ;xmm5 = b^2
mulsd xmm4, xmm4 ;xmm4 = a^2
addsd xmm4, xmm5 ;xmm4 = a^2 + b^2


vptest xmm4, xmm10 ;Jezeli xmm4 > limitu - wyjdz z petli
jb computeBrightness ;Skok poza petle

inc rbx ;Inkrementuj rbx
cmp rbx, r9 ;Jezeli rbx rowne jest liczbie iteracji, skoncz petle
je computeBrightness ;Skok poza petle
jmp iterating ;Zapetl

computeBrightness: ;Oblicz wartosc piksela

xor r12, r12 ;Wyzeruj r12
cmp rbx, r9 ;Jezeli rbx jest rowny liczbie iteracji, wpisz zero do piksela
je writeData ;Skok do wpisu piksela
cvtsi2sd xmm0, rbx ;Wpisz licznik iteracji do xmm0
cvtsi2sd xmm1, r9d ;Wpisz liczbe iteracji do xmm1
divsd xmm0, xmm1 ;Podziel xmm0 przez xmm1
sqrtsd xmm0, xmm0 ;Pierwiastek kwadratowy z xmm0
movsd xmm1, maxValue ;Wpisz 255 do xmm1
mulsd xmm0, xmm1 ;Pomnoz xmm0 przez xmm1
cvtsd2si r12, xmm0 ;Wpisz xmm0 do r12

writeData: ;Zapis wynikow do tablicy i zapetlenie/powrot z funkcji

mov byte ptr[rbp][rdi], r12b ;Wpisz r12 pod rozpatrywany piksel
inc rdi ;Zwieksz indeks tablicy 
cmp rdi, r10 ;Jezeli osiagnieto rozmiar tablicy - wyjdz z petli
je return ;Skok do wyjscia z petli
inc r11 ;Zwieksz wspol. x
cmp r11, r13 ;Jezeli osiagnieto szerokosc obrazu, skocz do petli zewnetrznej
je yloop ;Skok do petli zewnetrznej
jmp xloop ;Skok do petli wewnetrznej

return: ;Wyjscie z funkcji
;Zdjecie starych wartosci rejestrow ze stosu
pop rbp
pop rdi
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rbx
pop rax
ret ;Wyjscie z funkcji
generateMandelbrotSet endp ;Koniec funkcji

;Funkcja generujaca zbior Julii
;Na podstawie dodatkowej liczby zespolonej i wspolrzednych piksela okreslane sa wspolczynniki a i b liczby zespolonej. Po osiagnieciu limitu 
; lub liczby iteracji okreslana jest wartosc piksela na podstawie liczby wykonan petli
;Parametry wejsciowe - wskaznik na strukture parametrow JuliaStruct
;Parametry wyjsciowe - brak
;Zmieniane flagi: ZR

generateJuliaSet proc 

push rax
push rbx
push r8
push r9
push r10
push r11
push r12
push r13
push rdi
push rbp

mov rbp, [rcx].JuliaStruct.arr ;Wpisz adres tablicy do rbp
mov r8d, [rcx].JuliaStruct.startY ;Wpisz startY do r8
mov r9d, [rcx].JuliaStruct.iter ;Wpisz ilosc iteracji do r9
mov r10d, [rcx].JuliaStruct.s ;Wpisz rozmiar do r10
mov r13d, [rcx].JuliaStruct.dim ;Wpisz wymiar do r13
movsd xmm6, [rcx].JuliaStruct.x ;Wpisz wartosc x do xmm6
movsd xmm8, [rcx].JuliaStruct.r ;Wpisz parametr rzeczywisty do xmm8
movsd xmm9, [rcx].JuliaStruct.im ;Wpisz parametr urojony do xmm9
movsd xmm10, [rcx].JuliaStruct.d ;Wpisz przyrost do xmm10
movsd xmm12, [rcx].JuliaStruct.depth ;Wpisz limit do xmm12

xor rdi, rdi ;Wyzeruj rdi
jmp juliaStart ;Rozpocznij petle

juliaYloop: ;Petle zewnetrzna

addsd xmm6, xmm10 ;Dodaj przyrost do x

juliaStart:

xor r11, r11 ;Wyzeruj wspol. x
movsd xmm7, [rcx].JuliaStruct.minimal ;Wpisz minimalna wartosc y do xmm7

juliaXloop:;Petle wewnetrzna

movapd xmm0, xmm7 ;Wpisz y do xmm0 - a
movapd xmm1, xmm6 ;Wpisz x do xmm1 - b
xor rbx, rbx ;Wyzeruj licznik petli

juliaIterations: ;Petla generujaca fraktal


movapd xmm2, xmm0 ;Wpisz a do xmm2 
mulsd xmm2, xmm1  ;xmm2 = ab
addsd xmm2, xmm2 ;xmm2 = 2ab
mulsd xmm0, xmm0 ;a = a^2
mulsd xmm1, xmm1 ;b = b^2
subsd xmm0, xmm1 ;a = a^2 - b^2
addsd xmm0, xmm8 ;a = a^2 - b^2 + parametr rzeczywisty
movapd xmm1, xmm2 ;b = 2ab
addsd xmm1, xmm9 ;b = 2ab + parametr urojony

movapd xmm3, xmm0 ;xmm3 = a
mulsd xmm3, xmm3 ;xmm3 = a^2
movapd xmm4, xmm1 ;xmm4 = b
mulsd xmm4, xmm4 ;xmm4 = b^2
addsd xmm3, xmm4 ;xmm3 = a^2 + b^2

vptest xmm3, xmm12 ;Jezeli xmm4 > limitu - wyjdz z petli
jb computeJuliaBrightness ;Skok poza petle
inc rbx ;Inkrementuj rbx
cmp rbx, r9 ;Jezeli rbx rowne jest liczbie iteracji, skoncz petle
je computeJuliaBrightness ;Skok poza petle
jmp juliaIterations ;Zapetl

computeJuliaBrightness: ;Oblicz jasnosc piksela

xor r12, r12 ;Wyzeruj r12
cmp rbx, r9 ;Jezeli rbx jest rowny liczbie iteracji, wpisz zero do piksela
je writeJuliaData ;Skok do wpisu piksela

cvtsi2sd xmm0, rbx ;Wpisz licznik iteracji do xmm0
cvtsi2sd xmm1, r9d ;Wpisz liczbe iteracji do xmm1
divsd xmm0, xmm1 ;Podziel xmm0 przez xmm1
sqrtsd xmm0, xmm0 ;Pierwiastek kwadratowy z xmm0
movsd xmm1, maxValue ;Wpisz 255 do xmm1
mulsd xmm0, xmm1 ;Pomnoz xmm0 przez xmm1
cvtsd2si r12, xmm0 ;Wpisz xmm0 do r12

writeJuliaData: ;Wpisanie wyniku do tablicy i zapetlenie/powrot z funkcji

mov byte ptr[rbp][rdi], r12b ;Wpisz wynik pod zadane miejsce w tablicy
inc rdi ;Zwieksz indeks tablicy
cmp rdi, r10 ;Sprawdz, czy indeks jest rowny rozmiarowi
je juliaReturn ;Jezeli tak - wyjdz z funkcji
inc r11 ;Zwieksz wspolrzedna x piksela
cmp r11, r13 ;Jezeli jest rowny szerokosci, wskocz do petli zewnetrznej

je juliaYloop ;Skok do petli zewnetrznej
addsd xmm7, xmm10 ;
jmp juliaXloop ;Skok do petli wewnetrznej

juliaReturn: ;Powrot z funkcji

;Zdjecie ze stosu starej zawartosci rejestrow
pop rbp
pop rdi
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rbx
pop rax

ret ;Powrot z funkcji

generateJuliaSet endp ;Koniec funkcji

end ;Koniec kodu

generateMandelbrotSet proc

;Odlozenie zawartosci rejestrow na stos
push rax
push rbx
push rdx
push r8
push r9
push r10
push r11
push r12
push r13
push rdi
push rbp

mov rbp, [rcx].MandelbrotStruct.arr ;Wpisz adres tablicy do rbp
mov r8d, [rcx].MandelbrotStruct.startY ;Wpisz startY do r8
mov r9d, [rcx].MandelbrotStruct.iter ;Wpisz ilosc iteracji do r9
mov r10d, [rcx].MandelbrotStruct.s ;Wpisz rozmiar do r10
mov r13d, [rcx].MandelbrotStruct.dim ;Wpisz wymiar do r13
movsd xmm8, [rcx].MandelbrotStruct.scale ;Wpisz skale do xmm8
movsd xmm10, [rcx].MandelbrotStruct.depth ;Wpisz limit do xmm10
movsd xmm9, negative ;Wpisz -1 do xmm9
mulsd xmm9, xmm8 ;Pomnoz skale przez -1

xor rdi, rdi ;Wyzeruj rdi
jmp start ;rozpocznij petle

yloop: ;Petla zewnetrzna

inc r8d ;Zwieksz wspolrzedna y

start: 
xor r11, r11 ;Wyzeruj wspolrzedna x

xloop: ;Petla wewnetrzna

;Oblicz wspolczynnik a liczby zespolonej

cvtsi2sd xmm2, r11d ;Przepisz wspol. x do xmm2
cvtsi2sd xmm3, r13d ;Przepisz wymiar do xmm3
movapd xmm4, xmm8 ;Przepisz -skale do xmm4
movapd xmm0, xmm9 ;Przepisz skale do xmm0
addsd xmm4, xmm4 ;Dwukrotnosc rejestru xmm4
divsd xmm4, xmm3 ;Podziel xmm4 przez wymiar 
mulsd xmm2, xmm4 ;Pomnoz wspol. x przez xmm4
addsd xmm0, xmm2 ;Dodaj do skali xmm2

;Oblicz wspolczynnik b liczby zespolonej

cvtsi2sd xmm2, r8d ;Wpisz do xmm2 wspol. y
movapd xmm4, xmm8 ;Przepisz -skale do xmm4
movapd xmm1, xmm9 ;Przepisz skale do xmm1
addsd xmm4, xmm4 ;Dwukrotnosc rejestru xmm4
divsd xmm4, xmm3  ;Podziel xmm4 przez wymiar 
mulsd xmm2, xmm4 ;Pomnoz wspol. y przez xmm4
addsd xmm1, xmm2 ;Dodaj do skali xmm2

movapd xmm2, xmm0 ;Wpisz pierwotnie obliczone a do xmm2
movapd xmm3, xmm1 ;Wpisz pierwotnie obliczone b do xmm3
xor rbx, rbx ;Wyzeruj rbx przed iteracja

iterating: ;Petla generujaca fraktal

movapd xmm4, xmm0 ;Wpisz a do xmm4
movapd xmm5, xmm1 ;Wpisz b do xmm5
mulsd xmm4, xmm4 ;Podnies a do kwadratu
mulsd xmm5, xmm5 ;Podnies b do kwadratu
subsd xmm4, xmm5 ;a^2 - b^2
addsd xmm4, xmm2 ;a^2 - b^2 + pierowtna a

movapd xmm5, xmm0 ;Wpisz a do xmm5
mulsd xmm5, xmm1 ;a * b
addsd xmm5, xmm5 ;2ab
addsd xmm5, xmm3 ;2ab + pierwotne b
movapd xmm0, xmm4 ;a = (a^2 - b^2) + pierwotne a
movapd xmm1, xmm5 ;b = 2ab + pierwotne b
mulsd xmm5, xmm5; ;xmm5 = b^2
mulsd xmm4, xmm4 ;xmm4 = a^2
addsd xmm4, xmm5 ;xmm4 = a^2 + b^2


vptest xmm4, xmm10 ;Jezeli xmm4 > limitu - wyjdz z petli
jb computeBrightness ;Skok poza petle

inc rbx ;Inkrementuj rbx
cmp rbx, r9 ;Jezeli rbx rowne jest liczbie iteracji, skoncz petle
je computeBrightness ;Skok poza petle
jmp iterating ;Zapetl

computeBrightness: ;Oblicz wartosc piksela

xor r12, r12 ;Wyzeruj r12
cmp rbx, r9 ;Jezeli rbx jest rowny liczbie iteracji, wpisz zero do piksela
je writeData ;Skok do wpisu piksela
cvtsi2sd xmm0, rbx ;Wpisz licznik iteracji do xmm0
cvtsi2sd xmm1, r9d ;Wpisz liczbe iteracji do xmm1
divsd xmm0, xmm1 ;Podziel xmm0 przez xmm1
sqrtsd xmm0, xmm0 ;Pierwiastek kwadratowy z xmm0
movsd xmm1, maxValue ;Wpisz 255 do xmm1
mulsd xmm0, xmm1 ;Pomnoz xmm0 przez xmm1
cvtsd2si r12, xmm0 ;Wpisz xmm0 do r12

writeData: ;Zapis wynikow do tablicy i zapetlenie/powrot z funkcji

mov byte ptr[rbp][rdi], r12b ;Wpisz r12 pod rozpatrywany piksel
inc rdi ;Zwieksz indeks tablicy 
cmp rdi, r10 ;Jezeli osiagnieto rozmiar tablicy - wyjdz z petli
je return ;Skok do wyjscia z petli
inc r11 ;Zwieksz wspol. x
cmp r11, r13 ;Jezeli osiagnieto szerokosc obrazu, skocz do petli zewnetrznej
je yloop ;Skok do petli zewnetrznej
jmp xloop ;Skok do petli wewnetrznej

return: ;Wyjscie z funkcji
;Zdjecie starych wartosci rejestrow ze stosu
pop rbp
pop rdi
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rbx
pop rax
ret ;Wyjscie z funkcji
generateMandelbrotSet endp ;Koniec funkcji

;Funkcja generujaca zbior Julii
;Na podstawie dodatkowej liczby zespolonej i wspolrzednych piksela okreslane sa wspolczynniki a i b liczby zespolonej. Po osiagnieciu limitu 
; lub liczby iteracji okreslana jest wartosc piksela na podstawie liczby wykonan petli
;Parametry wejsciowe - wskaznik na strukture parametrow JuliaStruct
;Parametry wyjsciowe - brak
;Zmieniane flagi: ZR

generateJuliaSet proc 

push rax
push rbx
push r8
push r9
push r10
push r11
push r12
push r13
push rdi
push rbp

mov rbp, [rcx].JuliaStruct.arr ;Wpisz adres tablicy do rbp
mov r8d, [rcx].JuliaStruct.startY ;Wpisz startY do r8
mov r9d, [rcx].JuliaStruct.iter ;Wpisz ilosc iteracji do r9
mov r10d, [rcx].JuliaStruct.s ;Wpisz rozmiar do r10
mov r13d, [rcx].JuliaStruct.dim ;Wpisz wymiar do r13
movsd xmm6, [rcx].JuliaStruct.x ;Wpisz wartosc x do xmm6
movsd xmm8, [rcx].JuliaStruct.r ;Wpisz parametr rzeczywisty do xmm8
movsd xmm9, [rcx].JuliaStruct.im ;Wpisz parametr urojony do xmm9
movsd xmm10, [rcx].JuliaStruct.d ;Wpisz przyrost do xmm10
movsd xmm12, [rcx].JuliaStruct.depth ;Wpisz limit do xmm12

xor rdi, rdi ;Wyzeruj rdi
jmp juliaStart ;Rozpocznij petle

juliaYloop: ;Petle zewnetrzna

addsd xmm6, xmm10 ;Dodaj przyrost do x

juliaStart:

xor r11, r11 ;Wyzeruj wspol. x
movsd xmm7, [rcx].JuliaStruct.minimal ;Wpisz minimalna wartosc y do xmm7

juliaXloop:;Petle wewnetrzna

movapd xmm0, xmm7 ;Wpisz y do xmm0 - a
movapd xmm1, xmm6 ;Wpisz x do xmm1 - b
xor rbx, rbx ;Wyzeruj licznik petli

juliaIterations: ;Petla generujaca fraktal


movapd xmm2, xmm0 ;Wpisz a do xmm2 
mulsd xmm2, xmm1  ;xmm2 = ab
addsd xmm2, xmm2 ;xmm2 = 2ab
mulsd xmm0, xmm0 ;a = a^2
mulsd xmm1, xmm1 ;b = b^2
subsd xmm0, xmm1 ;a = a^2 - b^2
addsd xmm0, xmm8 ;a = a^2 - b^2 + parametr rzeczywisty
movapd xmm1, xmm2 ;b = 2ab
addsd xmm1, xmm9 ;b = 2ab + parametr urojony

movapd xmm3, xmm0 ;xmm3 = a
mulsd xmm3, xmm3 ;xmm3 = a^2
movapd xmm4, xmm1 ;xmm4 = b
mulsd xmm4, xmm4 ;xmm4 = b^2
addsd xmm3, xmm4 ;xmm3 = a^2 + b^2

vptest xmm3, xmm12 ;Jezeli xmm4 > limitu - wyjdz z petli
jb computeJuliaBrightness ;Skok poza petle
inc rbx ;Inkrementuj rbx
cmp rbx, r9 ;Jezeli rbx rowne jest liczbie iteracji, skoncz petle
je computeJuliaBrightness ;Skok poza petle
jmp juliaIterations ;Zapetl

computeJuliaBrightness: ;Oblicz jasnosc piksela

xor r12, r12 ;Wyzeruj r12
cmp rbx, r9 ;Jezeli rbx jest rowny liczbie iteracji, wpisz zero do piksela
je writeJuliaData ;Skok do wpisu piksela

cvtsi2sd xmm0, rbx ;Wpisz licznik iteracji do xmm0
cvtsi2sd xmm1, r9d ;Wpisz liczbe iteracji do xmm1
divsd xmm0, xmm1 ;Podziel xmm0 przez xmm1
sqrtsd xmm0, xmm0 ;Pierwiastek kwadratowy z xmm0
movsd xmm1, maxValue ;Wpisz 255 do xmm1
mulsd xmm0, xmm1 ;Pomnoz xmm0 przez xmm1
cvtsd2si r12, xmm0 ;Wpisz xmm0 do r12

writeJuliaData: ;Wpisanie wyniku do tablicy i zapetlenie/powrot z funkcji

mov byte ptr[rbp][rdi], r12b ;Wpisz wynik pod zadane miejsce w tablicy
inc rdi ;Zwieksz indeks tablicy
cmp rdi, r10 ;Sprawdz, czy indeks jest rowny rozmiarowi
je juliaReturn ;Jezeli tak - wyjdz z funkcji
inc r11 ;Zwieksz wspolrzedna x piksela
cmp r11, r13 ;Jezeli jest rowny szerokosci, wskocz do petli zewnetrznej

je juliaYloop ;Skok do petli zewnetrznej
addsd xmm7, xmm10 ;
jmp juliaXloop ;Skok do petli wewnetrznej

juliaReturn: ;Powrot z funkcji

;Zdjecie ze stosu starej zawartosci rejestrow
pop rbp
pop rdi
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rbx
pop rax

ret ;Powrot z funkcji

generateJuliaSet endp ;Koniec funkcji
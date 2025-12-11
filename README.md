KATARZYNA PACAK projekt_informatyka_2025_26
Arkanoid (C++ / SFML)

Opis

Projekt jest prostą grą zręcznościową typu Arkanoid napisaną w C++ z
wykorzystaniem biblioteki **SFML**.  
Gracz steruje paletką na dole ekranu i odbija piłkę tak,
aby zbić jak najwięcej klocków.

Na początku każdej rozgrywki przez ekran powoli spada żółta gwiazda.
W lewym górnym rogu wyświetlany jest licznik zbitych klocków.

Sterowanie

- A / strzałka w lewo – ruch paletki w lewo  
- D / strzałka w prawo – ruch paletki w prawo  
- ENTER – wybór opcji w menu  
- P – pauza / powrót z pauzy  
- ESC – w trakcie gry: zapis wyniku i powrót do menu  
- F5 – zapis aktualnego stanu gry do pliku `zapis.txt`

Ekrany

1. Menu główne
   - Nowa gra
   - Wczytaj grę (odczyt z pliku `zapis.txt`)
   - Ostatnie wyniki (odczyt z pliku `wyniki.txt`)
   - Wyjście

2. Gra
   - Piłka, paletka, rzędy klocków o różnej wytrzymałości
   - Spadająca gwiazda na początku
   - Licznik zbitych klocków w lewym górnym rogu

3. Pauza
   - Migający napis „PAUZA” 

4. Ekran wyników
   - Wyświetla do 10 ostatnich wyników (liczbę zbitych klocków)

Funkcje dodatkowe

- Zapis i wczytanie stanu gry (pozycja paletki, piłki, stan klocków).  
- Zapis historii wyników do pliku tekstowego.  
- Prosta animacja spadającej gwiazdy.  
- Tekstowy licznik zbitych klocków aktualizowany w czasie rzeczywistym.

Struktura projektu (najważniejsze pliki)

- `main.cpp` – pętla główna, obsługa stanów gry i okna SFML  
- `Game.h / Game.cpp` – logika gry, aktualizacja i rysowanie obiektów  
- `Ball.h` – klasa piłki (ruch, kolizje ze ścianami i paletką)  
- `Paddle.h` – klasa paletki sterowanej klawiaturą  
- `Brick.h`, `Bricks.h` – pojedynczy klocek i kolekcja klocków  
- `Star.h` – spadająca gwiazda na początku gry  
- `Menu.h / Menu.cpp` – tekstowe menu główne  
- `GameState.h / GameState.cpp` – zapis / odczyt stanu gry do pliku  
- `wyniki.txt` – plik z listą ostatnich wyników  
- `zapis.txt` – plik z zapisanym stanem gry

Wykorzystane mechanizmy 

- Programowanie obiektowe: klasy, enkapsulacja, metody.  
- Dziedziny SFML: grafika, tekst, zdarzenia z klawiatury.  
- Struktury danych: `std::vector` do przechowywania klocków i wyników.  
- Przekazywanie parametrów przez referencję (np. przy zapisie stanu gry).  
- Zapis/odczyt plików tekstowych (`std::ifstream`, `std::ofstream`).  
- Podział kodu na pliki nagłówkowe i źródłowe, system stanów gry.

Uruchamianie

Projekt był tworzony w Visual Studio.  
Do kompilacji potrzebna jest biblioteka SFML. 
Czcionka jest ładowana z systemowej ścieżki:

```cpp
"C:\\Windows\\Fonts\\arial.ttf"

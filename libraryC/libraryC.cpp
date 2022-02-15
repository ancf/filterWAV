/*
	Temat projektu: Filtr uśredniający WAV (średnia krocząca)
	Semestr: 5
	Rok akademicki: 2021/2022
	Opis algorytmu:
		Prosta średnia ruchoma - średnia arytmetyczna z ostatnich n pomiarów
	Autor: Krystian Stebel
*/

#include "pch.h"
#include "framework.h"
#include "libraryC.h"

/*
	Parametry wejściowe: 
		- wskaźniki na tablice z próbkami - oryginalną (tylko do odczytu) i kopię (do zapisywania wyników)
		- indeksy początkowy i końcowy przedziału do przetworzenia
*/
void filterC(short * tab, short * copy, unsigned int min, unsigned int max)
{
	short sum = 0;
	for (int j = 0; j < 8; j++) {
			sum += (tab[min + j] / 8);
	}
	copy[min + 8] = sum;
	for (unsigned int i = min+1; i < max-8; i++) {
		sum -= tab[i - 1] / 8;
		sum += tab[i + 7] / 8;
		copy[i+8] = sum;       
    }
}
/*
	Parametry wejściowe:
		- wskaźniki na tablice z próbkami - oryginalną (tylko do odczytu) i kopię (do zapisywania wyników)
		- indeksy początkowy i końcowy przedziału do przetworzenia
*/
void altFilterC(short * tab, short * copy, unsigned int min, unsigned int max)
{
	for (int i = min; i < max - 8; i++) {
		short sum = 0;
		for (int j = 0; j < 8; j++) {
			sum += tab[i + j] / 8;
		}
		copy[i + 8] = sum;
	}
}


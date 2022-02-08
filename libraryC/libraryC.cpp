// libraryC.cpp : Definiuje eksportowane funkcje dla biblioteki DLL.
//

#include "pch.h"
#include "framework.h"
#include "libraryC.h"


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



// libraryC.cpp : Definiuje eksportowane funkcje dla biblioteki DLL.
//

#include "pch.h"
#include "framework.h"
#include "libraryC.h"


void filterC(int * tab, int * copy, unsigned int min, unsigned int max)
{
   
  

    for (int i = min; i < max-4; i++) {
        long long sum = 0;
        
        for (int j = 0; j < 4; j++) {
            sum += tab[i + j];
        }
       copy[i+4] = sum / 4;
       
    }
}



// libraryC.cpp : Definiuje eksportowane funkcje dla biblioteki DLL.
//

#include "pch.h"
#include "framework.h"
#include "libraryC.h"


void FilterC(double * tab, long min, long max)
{
    double * copy = tab;
    double * modified = tab;
    for (int i = min; i < max; i++) {
        double sum = 0;
        
        for (int j = 0; j < 8; j++) {
            sum += copy[i - j];
        }
        modified[i] = sum / 8;
    }
    tab = modified;
}



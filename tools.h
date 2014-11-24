/*! \file tools.h
\brief fonctions usuelles
*/
#ifndef TOOLS
#define TOOLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int entierAleatoireEntreBorne(int borneInf, int borneSup);
char* extractSubstring(char* str, int beginIndex, int endIndex);
#endif

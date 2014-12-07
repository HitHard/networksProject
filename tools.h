/*! \file tools.h
\brief fonctions usuelles
*/
#ifndef TOOLS
#define TOOLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	char type;
	char code;
	int taille;
	char* fonction;
} trame;

int entierAleatoireEntreBorne(int borneInf, int borneSup);
char* extractSubstring(char* str, int beginIndex, int endIndex);
trame* extractDatas(char* rawTrame);
char* writeTrame(trame* datas);
#endif
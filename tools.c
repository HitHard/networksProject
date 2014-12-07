#include "tools.h"
int entierAleatoireEntreBorne(int borneInf, int borneSup) {
	static int initialise = 0;
	if(!initialise) {
		srand(time(NULL));
		initialise = 1;
	}
	return (rand()%(borneSup - borneInf)) + borneInf;
}

char* extractSubstring(char* str, int beginIndex, int endIndex) {
	char* buffer = (char*) malloc(BUFSIZ*sizeof(char));
<<<<<<< HEAD
	strncpy(buffer, str + beginIndex, endIndex - beginIndex);
=======
	char* p; char * b;
	for(p = (str + beginIndex), b = buffer; p < (str + endIndex); p++, b++)
        *b = *p;
    *(b + 1) = '\0';
>>>>>>> upstream/master
	return buffer;
}

trame* extractDatas(char* rawTrame) {
	trame* donnees = (trame*) malloc (sizeof (trame));

<<<<<<< HEAD
	char* idStr = extractSubstring(rawTrame, 2, 12);
	char* tailleStr = extractSubstring(rawTrame, 3, 15);

	donnees->type = rawTrame[0];
	donnees->code = rawTrame[1];
	donnees->id = atoi(idStr);
	donnees->taille = atoi(tailleStr);
	donnees->fonction = extractSubstring(rawTrame, 15,15 + (donnees->taille));

	free(idStr);
	free(tailleStr);

	return donnees;
=======
	char* tailleStr = extractSubstring(rawTrame, 2, 5);

	donnees->type = rawTrame[0];
	donnees->code = rawTrame[1];
	donnees->taille = atoi(tailleStr);
	donnees->fonction = extractSubstring(rawTrame, 5,5 + (donnees->taille));

	free(tailleStr);

	return donnees;
}

char* writeTrame(trame* datas) {
    char* buffer = (char*) malloc (BUFSIZ * sizeof(char));
    snprintf(buffer, BUFSIZ, "%c%c%03d%s%c", datas->type, datas->code, strlen(datas->fonction), datas->fonction, '\0');
    return buffer;
>>>>>>> upstream/master
}

char* writeTrame(trame* datas) {
	char* buffer = (char*) malloc (BUFSIZ * sizeof(char));
	snprintf(buffer, BUFSIZ, "%c%c%010d%03d%s%c", datas->type, datas->code, datas->id, strlen(datas->fonction), datas->fonction, '\0');
	return buffer;
}

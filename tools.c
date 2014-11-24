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
    strncpy(buffer, str + beginIndex, endIndex - beginIndex);
    return buffer;
}

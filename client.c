#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "networkFunctions.h"

int extraireEntierNaturel(char* entierNaturelStr) {
    int entierNaturel = atoi(entierNaturelStr);
    if(entierNaturel < 0) {
        return -1;
    }
    return entierNaturel;
}

int main(int argc, char* argv[]) {
    //arguments : nomProgramme(0) protocole(1) adresseDistante(2) portDistant(3) protocoleSession(4)
    if(argc < 2) {
        perror("Protocole non specifié (0->TCP, 1->UDP)");
        exit(-1);
    } else if(argc < 3) {
        perror("Adresse distante non specifiée");
        exit(-1);
    } else if(argc < 4) {
        perror("Port distant non specifié");
        exit(-1);
    } else if(argc < 5) {
        perror("Protocole session a utiliser non spécifié (0->Test)");
        exit(-1);
    } else if(argc > 5) {
        perror("Trop d'arguments");
        exit(-1);
    }

    int protocoleTypes[2] = {SOCK_STREAM, SOCK_DGRAM};
    int protocole = extraireEntierNaturel(argv[1]);
    int portDistant = extraireEntierNaturel(argv[3]);
    int procoleSession = extraireEntierNaturel(argv[4]);

    if(protocole < 0 || protocole > 1) {
        perror("Protocole specifié non valide (0->TCP, 1->UDP)");
        exit(-1);
    }
    if(portDistant < 0) {
        perror("Port spécifié non valide");
        exit(-1);
    }
    if(procoleSession < 0) {
        perror("Protocole session a utiliser non valide");
        exit(-1);
    }


    if(clientLoop(protocoleTypes[protocole], argv[2], portDistant, procoleSession) < 0) {
        exit(-1);
    }
    exit(0);
}

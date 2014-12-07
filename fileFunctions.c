window#include "fileFunctions.h"

/***************************
		OuvrirEnEcriture
***************************/
int OuvrirEnEcriture(FILE** ptr_fichier, char* nom_fichier) {
	if ((*ptr_fichier = fopen(nom_fichier, "w")) == NULL) {
		return -1;
	}
	return 0;
}

/***************************
		OuvrirEnAppend
***************************/
int OuvrirEnAppend(FILE** ptr_fichier, char* nom_fichier) {
	if ((*ptr_fichier = fopen(nom_fichier, "a+")) == NULL) {
		return -1;
	}
	return 0;
}

/***************************
		OuvrirEnLecture
***************************/
int OuvrirEnLecture(FILE** ptr_fichier, char* nom_fichier) {
	if ((*ptr_fichier = fopen(nom_fichier, "r")) == NULL) {
		return -1;
	}
	return 0;
}

/***************************
		FermerFichier
***************************/
int FermerFichier(FILE* fichier) {
	if(fclose(fichier) != 0) {
		return -1;
	}
	return 0;
}

/***************************
		EcrireLigne
***************************/
int EcrireLigne(FILE* fichier, char* chaine) {
	if (fprintf(fichier,"%s", chaine) < 0) {
		return -1;
	}
	return 0;
}

/***************************
		LireLigne
***************************/
int LireLigne(FILE* fichier, int ligne, char* buffer) {
	rewind(fichier);
	int i = 1;
	while(fgets(buffer, BUFSIZ, fichier) != NULL && i < ligne) {
		i++;
	}
	if(i < ligne)
        return -1;
    return 0;
}

/***************************
		NombreLigne
***************************/
int NombreLigne(FILE* fichier) {
	int i = 0;
	FILE* f = fichier;
	rewind(f);
	char* buffer = (char *) malloc(BUFSIZ * sizeof(char));
	while(fgets(buffer, BUFSIZ, f) != NULL) {
		i++;
	}
	free(buffer);
	return i;
}

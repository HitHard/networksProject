#include "fileFunctions.h"

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
	if ((*ptr_fichier = fopen(nom_fichier, "a")) == NULL) {
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
	if (fwrite(chaine, strlen(chaine), 1, fichier) == 0) {
		return -1;
	}
	return 0;
}

/***************************
		LireLigne
***************************/
int LireLigne(FILE* fichier, char* buffer) {
	if(fgets(buffer, BUFSIZ, fichier) == NULL) {
		return -1;
	}
	return 0;
}
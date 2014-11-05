/*! \file primitives.h
\brief Contient les primitives de gestions d'un fichier
*/
#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <stdio.h>
#include <string.h>



/**
* Ouvre un fichier en écriture (écrase le fichier s'il existait déjà)
* @param ptr_fichier : Adresse du pointeur de fichier
* @param nom_fichier : Nom du fichier à ouvrir
*
* @return Retourne 0 si l'ouverture s'est passé correctement, -1 sinon
*/
int OuvrirEnEcriture(FILE** ptr_fichier, char* nom_fichier);

/**
* Ouvre un fichier en écriture et se positionne à la fin
* @param ptr_fichier : Adresse du pointeur de fichier
* @param nom_fichier : Nom du fichier à ouvrir
*
* @return Retourne 0 si l'ouverture s'est passé correctement, -1 sinon
*/
int OuvrirEnAppend(FILE** ptr_fichier, char* nom_fichier);


/**
* Ouvre un fichier en lecture
* @param ptr_fichier : Adresse du pointeur de fichier
* @param nom_fichier : Nom du fichier à ouvrir
*
* @return Retourne 0 si l'ouverture s'est passé correctement, -1 sinon
*/
int OuvrirEnLecture(FILE** ptr_fichier, char* nom_fichier);


/**
* Ferme un fichier
* @param fichier : Pointeur de fichier à fermer
*
* @return Retourne 0 si la fermeture s'est passé correctement, -1 sinon
*/
int FermerFichier(FILE* fichier);


/**
* Ecrit une chaine de caractère dans un fichier
* @param fichier : Pointeur du fichier sur lequel l'écriture doit avoir lieu
* @param chaine : Chaine de caractère à écrire dans le fichier
*
* @return Retourne 0 si l'écriture s'est passé correctement, -1 sinon
*/
int EcrireLigne(FILE* fichier, char* chaine);


/**
* Lit une ligne d'un fichier
* @param fichier : Pointeur du fichier à lire
* @param buffer : Buffer dans lequel sera stocké la ligne lue
*
* @return Retourne 0 si la lecture s'est passé correctement, -1 sinon. Retourne également -1 en fin de fichier
*/
int LireLigne(FILE* fichier, char* buffer);

#endif

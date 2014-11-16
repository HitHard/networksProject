/*! \file protocolHandlers.h
\brief fonctions de traitement des protocoles
*/
#ifndef PROTOCOL_HANDLERS
#define PROTOCOL_HANDLERS
#include <stdio.h>          // Fichier contenant les en-t�tes des fonctions standard d'entr�es/sorties
#include <stdlib.h>         // Fichier contenant les en-t�tes de fonctions standard telles que malloc()
#include <string.h>         // Fichier contenant les en-t�tes de fonctions standard de gestion de cha�nes de caract�res
#include <unistd.h>         // Fichier d'en-t�tes de fonctions de la norme POSIX (dont gestion des fichiers : write(), close(), ...)
#include <sys/types.h>      // Fichier d'en-t�tes contenant la d�finition de plusieurs types et de structures primitifs (syst�me)
#include <sys/socket.h>     // Fichier d'en-t�tes des fonctions de gestion de sockets
#include <netinet/in.h>     // Fichier contenant diff�rentes macros et constantes facilitant l'utilisation du protocole IP
#include <netdb.h>          // Fichier d'en-t�tes contenant la d�finition de fonctions et de structures permettant d'obtenir des informations sur le r�seau (gethostbyname(), struct hostent, ...)
#include <memory.h>         // Contient l'inclusion de string.h (s'il n'est pas d�j� inclus) et de features.h
#include <errno.h>          // Fichier d'en-t�tes pour la gestion des erreurs (notamment perror())

/**
* Fonction de traitement - protocole Test
* Lit le message de la requete et genere une reponse contenant ce message suffixe par " - OK"
* @param request : chaine de caractere de la requete
*
* @return la chaine de caractere de la reponse (request + " - OK")
*/
char* handleTestRequest(char* request);

#endif

/*! \file protocolHandlers.h
\brief fonctions de traitement des protocoles
*/
#ifndef PROTOCOL_HANDLERS
#define PROTOCOL_HANDLERS
#include <stdio.h>          // Fichier contenant les en-têtes des fonctions standard d'entrées/sorties
#include <stdlib.h>         // Fichier contenant les en-têtes de fonctions standard telles que malloc()
#include <string.h>         // Fichier contenant les en-têtes de fonctions standard de gestion de chaînes de caractères
#include <unistd.h>         // Fichier d'en-têtes de fonctions de la norme POSIX (dont gestion des fichiers : write(), close(), ...)
#include <sys/types.h>      // Fichier d'en-têtes contenant la définition de plusieurs types et de structures primitifs (système)
#include <sys/socket.h>     // Fichier d'en-têtes des fonctions de gestion de sockets
#include <netinet/in.h>     // Fichier contenant différentes macros et constantes facilitant l'utilisation du protocole IP
#include <netdb.h>          // Fichier d'en-têtes contenant la définition de fonctions et de structures permettant d'obtenir des informations sur le réseau (gethostbyname(), struct hostent, ...)
#include <memory.h>         // Contient l'inclusion de string.h (s'il n'est pas déjà inclus) et de features.h
#include <errno.h>          // Fichier d'en-têtes pour la gestion des erreurs (notamment perror())

/**
* Fonction de traitement - protocole Test
* Lit le message de la requete et genere une reponse contenant ce message suffixe par " - OK"
* @param request : chaine de caractere de la requete
*
* @return la chaine de caractere de la reponse (request + " - OK")
*/
char* handleTestRequest(char* request);

#endif

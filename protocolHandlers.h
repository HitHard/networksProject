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
#include <time.h>
#include "tools.h"
#include "fileFunctions.h"

/**
* Fonction d'initialisation de la mémoire partagée pour le protocole Test
*
* @return 0 si l'initialisation reussie, -1 en cas d'erreur
*/
int testSharedInitializer(void);

/**
* Fonction de nettoyage de la mémoire partagée pour le protocole Test
*
* @return 0 si l'initialisation reussie, -1 en cas d'erreur
*/
int testSharedCleaner(void);

/**
* Fonction de traitement cote serveur - protocole Test
* Lit le message de la requete et genere une reponse contenant ce message suffixe par " - OK"
* @param request : chaine de caractere de la requete
*
* @return la chaine de caractere de la reponse (request + " - OK")
*/
char* handleTestRequest(char* request);

/**
* Fonction de generation de requete cote client - protocole Test
* Genere un message contenant le pid et le temps local
*
* @return la chaine de caractere de la requete
*/
char* generateTestRequest();

/**
* Fonction de generation de reponse cote client - protocole Test
* Affiche simplement le message retourne par le serveur
*
* @param answer : la chaine de caractere de la reponse du serveur
* @return 0
*/
int handleTestAnswer(char * answer);

/**
* Fonction d'initialisation de la mémoire partagée pour le protocole CSMA CD
*
* @return 0 si l'initialisation reussie, -1 en cas d'erreur
*/
int csmaCDSharedInitializer(void);

/**
* Fonction de nettoyage de la mémoire partagée pour le protocole CSMA CD
*
* @return 0 si l'initialisation reussie, -1 en cas d'erreur
*/
int csmaCDSharedCleaner(void);

/**
* Fonction de traitement cote serveur - protocole CSMA CD
* Lit le message de la requete et genere une reponse contenant ce message suffixe par " - OK"
* @param request : chaine de caractere de la requete
*
* @return la chaine de caractere de la reponse (request + " - OK")
*/
char* handleCsmaCDRequest(char* request);

/**
* Fonction de generation de requete cote client - protocole CSMA CD
* Genere un message contenant le pid et le temps local
*
* @return la chaine de caractere de la requete
*/
char* generateCsmaCDRequest();

/**
* Fonction de generation de reponse cote client - protocole CSMA CD
* Affiche simplement le message retourne par le serveur
*
* @param answer : la chaine de caractere de la reponse du serveur
* @return 0
*/
int handleCsmaCDAnswer(char * answer);

#endif

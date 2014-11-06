/*! \file networkFunctions.h
\brief fonctions usuelles reseaux (ouverture de connexion, envoie de donnees....)
*/
#ifndef NETWORK_FUNCTIONS
#define NETWORK_FUNCTIONS
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
* Créer une socket liee au port de numero "port"
* @param port : le numero du port auquel sera liee la socket
* @param typeSocket : type de la socket a creer (generalement SOCK_STREAM [TCP] ou SOCK_DGRAM [UDP])
*
* @return Retourne le descripteur de la socket (>0) si la creation s'est passe correctement, -1 sinon
*/
int creerSocket(u_short port, int typeSocket);


/**
* Traite une requete TCP arrivee sur la socket pointee par le pointeur "socket"
* @param socket : le pointeur pointant la socket sur laquelle est arrivee la requete
*
* @return Retourne 0 si le traitement de la requete s'est passe correctement, -1 sinon
*/
//int traiterRequeteTCP(int* socket);

/**
* Traite une requete UDP arrivee sur la socket pointee par le pointeur "socket"
* @param socket : le pointeur pointant la socket sur laquelle est arrivee la requete
*
* @return Retourne 0 si le traitement de la requete s'est passe correctement, -1 sinon
*/
//int traiterRequeteUDP(int* socket);

/**
* Fonction d'intialistion du serveur :
* Premiere etape :
* Creation des sockets TCP et UDP a partir du numero de port "portInitial"
* Ainsi si l'on souhaite creer n sockets TCP et m sockets UDP a partir du port k, les sockets TCP seront ouvertes sur les ports [k..k+n] et les sockets UDP sur les ports [k+n+1..k+n+1+m]
*
* Deuxieme etape :
* La fonction initialise la boucle principale du serveur qui repondra aux requetes arrivant sur les sockets à l'aide des fonction de traitements TCP et UDP
*
* @param nbSocketsTCP : nombre de sockets TCP a ouvrir
* @param nbSocketsUDP : nombre de sockets UDP a ouvrir
* @param portInitial : numero de port a partir duquel les sockets seront initialisees
*
* @return Retourne -1 si une erreur survient
*/
int serverLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial);

#endif

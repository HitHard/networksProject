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
#include "protocolHandlers.h"
#include "tools.h"

/**
* Créer une socket liee au port de numero "port"
* @param port : le numero du port auquel sera liee la socket
* @param typeSocket : type de la socket a creer (generalement SOCK_STREAM [TCP] ou SOCK_DGRAM [UDP])
* @param isListener : indique si la socket est une socket d'ecoute
*
* @return Retourne le descripteur de la socket (>0) si la creation s'est passe correctement, -1 sinon
*/
int creerSocket(u_short port, int typeSocket, int isListener);


/**
* Traite une requete TCP arrivee sur la socket pointee par le pointeur "socket"
* @param socket : le pointeur pointant la socket sur laquelle est arrivee la requete
* @param fonctionHandleRequest : pointeur sur la fonction d'analyse de la requete
*
* @return Retourne 0 si le traitement de la requete s'est passe correctement, -1 sinon
*/
int traiterRequeteTCP(int* socket, char* (fonctionHandleRequest)(char*));

/**
* Traite une requete UDP arrivee sur la socket pointee par le pointeur "socket"
* @param socket : le pointeur pointant la socket sur laquelle est arrivee la requete
* @param fonctionHandleRequest : pointeur sur la fonction d'analyse de la requete
*
* @return Retourne 0 si le traitement de la requete s'est passe correctement, -1 sinon
*/
int traiterRequeteUDP(int* socket, char* (fonctionHandleRequest)(char*));

/**
* Créé un fils et utilise la fonction passee en argument pour traiter la requete arrivee sur la socket
* @param socket : le pointeur pointant la socket sur laquelle est arrivee la requete
* @param fonctionTraitement : pointeur sur la fonction qui traitera la requete (disinction TCP/UDP)
* @param fonctionHandleRequest : pointeur sur la fonction d'analyse de la requete
*
* @return Retourne 0 si le traitement de la requete s'est passe correctement, -1 sinon
*/
int processRequest(int* socket, int (*fonctionTraitement)(int*, char* (fonctionHandleRequest)(char*)), char* (fonctionHandleRequest)(char*));

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
* @param protocolHandlerId : identifiant du gestionnaire de protocole (Protocole de Test = 0,...)
*
* @return Retourne -1 si une erreur survient
*/
int serverLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial, int protocolHandlerId);

/**
* Fonction d'intialistion du client:
* Initialise la socket, se connecte au serveur puis, effectue des requetes aupres de ce dernier puis traite les reponses
*
* @param protocolType : type du protocole (SOCK_STREAM, SOCK_DGRAM)
* @param nomDistant : nom distant (ou adresse) du serveur
* @param portDistant : port du serveur
* @param protocolHandlerId : identifiant du gestionnaire de protocole (Protocole de Test = 0,...)
*
* @return Retourne -1 si une erreur survient, 0 sinon
*/
int clientLoop(int protocolType, char* nomDistant, u_short portDistant, int protocolHandlerId);

/**
* Fonction d'envoie de requete du client (version UDP)
* Genere une requete a l'aide de la fonction fonctionGenerateRequest, l'envoie au serveur, recupere la reponse et la traite avec la fonction fonctionHandleAnswer
*
* @param socket : socket sur laquelle les donnees sont echangees
* @param adresseDistante : structure representant l'adresse et le port du serveur distant
* @param fonctionGenerateRequest : pointeur sur la fonction permettant de generer la requete
* @param fonctionHandleAnswer : pointeur sur la fonction permettant de traiter la reponse
*
* @return Retourne -1 si une erreur survient, 0 sinon
*/
int envoyerRequeteUDP(int* socket, struct sockaddr_in* adresseDistante, char* (fonctionGenerateRequest)(void), int (fonctionHandleAnswer)(char *));

/**
* Fonction d'envoie de requete du client (version TCP)
* Genere une requete a l'aide de la fonction fonctionGenerateRequest, l'envoie au serveur, recupere la reponse et la traite avec la fonction fonctionHandleAnswer
*
* @param socket : socket sur laquelle les donnees sont echangees
* @param fonctionGenerateRequest : pointeur sur la fonction permettant de generer la requete
* @param fonctionHandleAnswer : pointeur sur la fonction permettant de traiter la reponse
*
* @return Retourne -1 si une erreur survient, 0 sinon
*/
int envoyerRequeteTCP(int* socket, struct sockaddr_in* adresseDistante, char* (fonctionGenerateRequest)(void), int (fonctionHandleAnswer)(char *));

#endif

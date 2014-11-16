#include "networkFunctions.h"
#define ATTENTE_MAX 5
#define PROTOCOL_HANDLERS_COUNT 1

/***************************
		CreerSocket
***************************/
int creerSocket(u_short port, int type) {
    int sock;
    int retour;
    struct sockaddr_in adresse;

    if( (sock = socket(AF_INET, type, 0)) < 0) {
        perror("Impossible d'ouvrir la socket\n");
        return -1;
    }

    adresse.sin_family      = AF_INET;
    adresse.sin_port        = htons(port);
    adresse.sin_addr.s_addr = INADDR_ANY;

    if( (retour = bind(sock, (struct sockaddr *) &adresse, sizeof adresse)) < 0) {
        perror("Nommage de la socket impossible\n");
        return -1;
    }

    return sock;
}

/***************************
		processRequest
***************************/
int processRequest(int* socket, int (*fonctionTraitement)(int*, char* (fonctionHandleRequest)(char*)), char* (fonctionHandleRequest)(char*)) {
    //Création d'un processus fils
    int pid = fork();
    if(pid < 0) {
        perror("Erreur fork");
        return -1;
    } else if(pid == 0) {
        //Code processus fils
        if((*fonctionTraitement)(socket, fonctionHandleRequest) < 0) {
            perror("Erreur traitement");
            exit(-1);
        } else {
            exit(0);
        }
        //Fin code processus fils
    }
    return 0;
}

/***************************
		traiterRequeteTCP
***************************/
int traiterRequeteTCP(int* socket, char* (fonctionHandleRequest)(char*)) {
    int socketEchanges;
    char request[BUFSIZ];
    char* answer = NULL;

    //Synchronisation de la connexion avec le client
    socketEchanges = accept (*socket, (struct sockaddr *) 0, (unsigned int*) 0);
    if (socketEchanges == -1) {
        perror("Erreur accept");
        return -1;
    } else {
        printf("Accept réussis\n");
        //Lecture des données envoyées par le client
        if(read(socketEchanges, request, BUFSIZ) == -1) {
            perror("Erreur read");
            return -1;
        } else {
            printf("Message reçu : %s\n",request);
            answer = fonctionHandleRequest(request);
            if(write(socketEchanges,answer,strlen(answer) + 1) == -1) {
                perror("Erreur write");
                return -1;
            } else {
                printf("Ecriture réussie\n");
            }
            free(answer);
        }
    }
    //Fermeture de la socket d'échanges
    close(socketEchanges);
    return 0;
}

/***************************
		traiterRequeteUDP
***************************/
int traiterRequeteUDP(int* socket, char* (fonctionHandleRequest)(char*)) {
    char* answer = NULL;
    char request[BUFSIZ];
    struct sockaddr_in from;
    int lenghtOfFrom;

    lenghtOfFrom = sizeof(from);
    if(recvfrom(*socket, request, 1024, 0, (struct sockaddr *) &from, (socklen_t *) &lenghtOfFrom) < 0) {
        perror("Erreur recvfrom");
        return -1;
    } else {
        printf("Message reçu : %s\n",request);
        answer = fonctionHandleRequest(request);
        if(sendto(*socket, answer, strlen(answer) + 1, 0, (struct sockaddr *) &from, (socklen_t) lenghtOfFrom) < 0) {
            perror("Erreur sendto");
            return -1;
        } else {
            printf("Ecriture réussie\n");
        }
        free(answer);
    }
    return 0;
}

/***************************
		serverLoop
***************************/
int serverLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial, int protocolHandlerId) {
    char* (*protocolHandlers[1])(char*) = {handleTestRequest};
    fd_set readFds;
    int descripteursSockets[nbSocketsTCP + nbSocketsUDP];
    int i;
    int nbSockets = nbSocketsTCP + nbSocketsUDP;

    if(protocolHandlerId < 0 || protocolHandlerId > PROTOCOL_HANDLERS_COUNT - 1) {
        printf("Identifiant du gestionnaire de protocole incorrect : %d\n", protocolHandlerId);
        return -1;
    }

    //Initialisation des sockets
    for(i = 0; i < nbSocketsTCP; i++) {
        if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_STREAM)) < 0) {
            perror("Erreur de création de socket TCP\n");
            return -1;
        }
        printf("Socket TCP n° %d creee sur le port %d\n", i, i + portInitial);
    }

    for(; i < nbSockets; i++) {
        if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_DGRAM)) < 0 ) {
            perror("Erreur de création de socket UDP\n");
            return -1;
        }
        printf("Socket UDP n° %d creee sur le port %d\n", i, i + portInitial);
    }
    //Fin initialisation des sockets

    //Demarrage des sockets
    for(i = 0; i < nbSockets; i++) {
        listen(descripteursSockets[i], ATTENTE_MAX);
        printf("Socket n° %d placee en mode ecoute\n", i);
    }

    //Boucle principale serveur
    while(1) {
        //Initialisation de l'ensemble des descripteurs à lire
        FD_ZERO(&readFds);
        printf("FD_ZERO\n");
        for(i = 0; i < nbSockets; i++) {
            printf("FD_SET %d\n", i);
            FD_SET(descripteursSockets[i], &readFds);
        }
        //Fin initialisation des descripteurs à lire

        if(select(descripteursSockets[nbSockets - 1] + 1, &readFds, 0, 0, 0) < 0) {
            perror("Erreur de lecture des descripteurs\n");
            break; //En cas d'erreur on quitte la boucle pour fermer les sockets
        }

        for(i = 0; i < nbSockets; i++) {
            printf("IS SET %d?\n", i);
            //TODO : retirer la ligne "sleep" (utilisée pour le debug)
            sleep(1);
            if(FD_ISSET(descripteursSockets[i], &readFds)) {
                if(i < nbSocketsTCP) {
                    printf("Requete TCP arrivée sur la socket n°%d\n", i);
                    processRequest(&descripteursSockets[i], traiterRequeteTCP, protocolHandlers[protocolHandlerId]);
                }
                else {
                    printf("Requete UDP arrivée sur la socket n°%d\n", i);
                    processRequest(&descripteursSockets[i], traiterRequeteUDP, protocolHandlers[protocolHandlerId]);
                }
            }
        }

    }
    //Fin boucle principale serveur

    //Fermeture des sockets
    for(i = 0; i < nbSockets; i++) {
        close(descripteursSockets[i]);
        printf("Socket n°%d fermee\n", i);
    }
    //Fin fermeture des sockets
}

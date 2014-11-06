#include "networkFunctions.h"
#define ATTENTE_MAX 5

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
		serverLoop
***************************/
int serverLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial) {
    fd_set readFds;
    int descripteursSockets[nbSocketsTCP + nbSocketsUDP];
    int i;
    int nbSockets = nbSocketsTCP + nbSocketsUDP;

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
            sleep(1);
            if(FD_ISSET(descripteursSockets[i], &readFds)) {
                if(i < nbSocketsTCP) {
                    printf("Requete TCP arrivée sur la socket n°%d\n", i);
                    //TODO traitement requete TCP
                }
                else {
                    printf("Requete UDP arrivée sur la socket n°%d\n", i);
                    //TODO traitement requete UDP
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

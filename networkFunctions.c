#include "networkFunctions.h"
#define ATTENTE_MAX 5
#define SLEEP_CLIENT_MIN 0
#define SLEEP_CLIENT_MAX 5
#define PROTOCOL_HANDLERS_COUNT 2

/***************************
		CreerSocket
***************************/
int creerSocket(u_short port, int type, int isListener) {
	int sock;
	struct sockaddr_in adresse;

	if( (sock = socket(AF_INET, type, 0)) < 0) {
		perror("Impossible d'ouvrir la socket\n");
		return -1;
	}

	adresse.sin_family      = AF_INET;
	adresse.sin_port        = htons(port);

	if(isListener) {
		adresse.sin_addr.s_addr = INADDR_ANY;

		if(bind(sock, (struct sockaddr *) &adresse, sizeof adresse) < 0) {
			perror("Nommage de la socket impossible\n");
			return -1;
		}
	}

	return sock;
}

/***************************
		processRequest
***************************/
int processRequest(int* socket, int (*fonctionTraitement)(int*, char* (fonctionHandleRequest)(char*)), char* (fonctionHandleRequest)(char*)) {
	if((*fonctionTraitement)(socket, fonctionHandleRequest) < 0) {
		  perror("Erreur traitement");
		  return(-1);
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
		//close(*socket);
		//while(1) {
			if(read(socketEchanges, request, BUFSIZ) < 0) {
				perror("Erreur read");
				return -1;
			} else {
				printf("Message reçu : %s\n",request);
				answer = fonctionHandleRequest(request);
				if(write(socketEchanges,answer,strlen(answer) + 1) < 0) {
					perror("Erreur write");
					free(answer);
					return -1;
				} else {
<<<<<<< HEAD
<<<<<<< HEAD
					printf("Ecriture réussie\n");
=======
					printf("Ecriture réussie : %s\n", answer);
>>>>>>> upstream/master
=======
					printf("Ecriture réussie : %s\n", answer);
>>>>>>> upstream/master
				}
				free(answer);
			}
		//}
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
	if(recvfrom(*socket, request, BUFSIZ, 0, (struct sockaddr *) &from, (socklen_t *) &lenghtOfFrom) < 0) {
		perror("Erreur recvfrom");
		return -1;
	} else {
		printf("Message reçu : %s\n",request);
		answer = fonctionHandleRequest(request);
		if(sendto(*socket, answer, strlen(answer) + 1, 0, (struct sockaddr *) &from, (socklen_t) lenghtOfFrom) < 0) {
			perror("Erreur sendto");
			free(answer);
			return -1;
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			printf("Ecriture réussie\n");
=======
			printf("Ecriture réussie : %s\n",answer);
>>>>>>> upstream/master
=======
			printf("Ecriture réussie : %s\n",answer);
>>>>>>> upstream/master
		}
		free(answer);
	}
	return 0;
}

/***************************
		serverLoop
***************************/
int serverLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial, int protocolHandlerId) {
	int (*protocolSharedInitializer[PROTOCOL_HANDLERS_COUNT])(void) = {testSharedInitializer, csmaCDSharedInitializer};
	int (*protocolSharedCleaner[PROTOCOL_HANDLERS_COUNT])(void) = {testSharedCleaner, csmaCDSharedCleaner};
	char* (*protocolHandlers[PROTOCOL_HANDLERS_COUNT])(char*) = {handleTestRequest, handleCsmaCDRequest};

	fd_set readFds;
	int descripteursSockets[nbSocketsTCP + nbSocketsUDP];
	int i;
	int nbSockets = nbSocketsTCP + nbSocketsUDP;

	if(protocolSharedInitializer[protocolHandlerId]() < 0) {
		perror("Erreur lors de l'initialisation de la mémoire partagée");
		return -1;
	}

	if(protocolHandlerId < 0 || protocolHandlerId > PROTOCOL_HANDLERS_COUNT - 1) {
		printf("Identifiant du gestionnaire de protocole incorrect : %d\n", protocolHandlerId);
		return -1;
	}

	//Initialisation des sockets
	for(i = 0; i < nbSocketsTCP; i++) {
		if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_STREAM, 1)) < 0) {
			perror("Erreur de création de socket TCP\n");
			return -1;
		}
		printf("Socket TCP n° %d creee sur le port %d\n", i, i + portInitial);
	}

	for(; i < nbSockets; i++) {
		if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_DGRAM, 1)) < 0 ) {
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
<<<<<<< HEAD
<<<<<<< HEAD
		printf("FD_ZERO\n");
		for(i = 0; i < nbSockets; i++) {
			printf("FD_SET %d\n", i);
=======
		for(i = 0; i < nbSockets; i++) {
>>>>>>> upstream/master
=======
		for(i = 0; i < nbSockets; i++) {
>>>>>>> upstream/master
			FD_SET(descripteursSockets[i], &readFds);
		}
		//Fin initialisation des descripteurs à lire

		if(select(descripteursSockets[nbSockets - 1] + 1, &readFds, 0, 0, 0) < 0) {
			perror("Erreur de lecture des descripteurs\n");
			break; //En cas d'erreur on quitte la boucle pour fermer les sockets
		}

		for(i = 0; i < nbSockets; i++) {
			//printf("IS SET %d?\n", i);
			//TODO : retirer la ligne "sleep" (utilisée pour le debug)
			//sleep(1);
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

	if(protocolSharedCleaner[protocolHandlerId]() < 0) {
		perror("Erreur lors du nettoyage de la mémoire partagée");
		return -1;
	}
	return 0;
}

/***************************
		serverPollingLoop
***************************/
int serverPollingLoop(u_short nbSocketsTCP, u_short nbSocketsUDP, u_short portInitial) {
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
=======

>>>>>>> upstream/master
	clientUDP clUDP;

	fd_set readFds;
	int descripteursSockets[nbSocketsTCP + nbSocketsUDP];
	int i;
	int nbSockets = nbSocketsTCP + nbSocketsUDP;

	int nbClientTCP = 0;
	int* listeClientTCP = (int*) calloc(nbSocketsTCP, sizeof(int));
	int nbClientUDP = 0;
	clientUDP* listeClientUDP = (clientUDP*) calloc(nbSockets, sizeof(clientUDP));

	int sockTCP;
	char* serveurRequest = "hr";
	char buffer[BUFSIZ];
	char* answer = NULL;
	char request[BUFSIZ];

	struct timeval temps;
	temps.tv_sec = 0;
	temps.tv_usec = 0;


	//Initialisation des sockets
	for(i = 0; i < nbSocketsTCP; i++) {
		if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_STREAM, 1)) < 0) {
			perror("Erreur de création de socket TCP\n");
			return -1;
		}
		printf("Socket TCP n° %d creee sur le port %d\n", i, i + portInitial);
	}

	for(; i < nbSockets; i++) {
		if( (descripteursSockets[i] = creerSocket(portInitial + i, SOCK_DGRAM, 1)) < 0 ) {
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
<<<<<<< HEAD
<<<<<<< HEAD
		printf("FD_ZERO\n");
		for(i = 0; i < nbSockets; i++) {
			printf("FD_SET %d\n", i);
=======
		for(i = 0; i < nbSockets; i++) {
>>>>>>> upstream/master
=======
		for(i = 0; i < nbSockets; i++) {
>>>>>>> upstream/master
			FD_SET(descripteursSockets[i], &readFds);
		}
		//Fin initialisation des descripteurs à lire

		//On demande à chaque client TCP s'ils ont une requete et on l'execute
		for(i = 0; i < nbClientTCP; i++) {
			memset(request, 0, sizeof(request));
			printf("Polling socket TCP %d\n", listeClientTCP[i]);
			if(write(listeClientTCP[i], serveurRequest, strlen(serveurRequest)) == -1) {
				perror("Erreur 1e write");
			}
			else {
				if(read(listeClientTCP[i], request, BUFSIZ) == -1 ) {
					perror("Erreur read");
				}
				else {
					printf("Recu : %s\n", request);
					if(answer != NULL) memset(answer, 0, sizeof(answer));
					answer = handlePollingRequest(request);
					if(write(listeClientTCP[i], answer, strlen(answer)) == -1) {
						perror("Erreur 2e write");
					}
				}
			}
		}

		//On demande à chaque client UDP s'ils ont une requete et on l'execute
		for(i = 0; i < nbClientUDP; i++) {
			memset(request, 0, sizeof(request));
			printf("Polling socket UDP %d\n", listeClientUDP[i].socket);
			if(sendto(listeClientUDP[i].socket, serveurRequest, strlen(serveurRequest), 0, (struct sockaddr *)&(listeClientUDP[i].adresse), sizeof(listeClientUDP[i].adresse)) == -1) {
				perror("Erreur 1e sendto");
			}
			else {
				int lenghtOfAdresse = sizeof(listeClientUDP[i].adresse);
				if(recvfrom(listeClientUDP[i].socket, request, BUFSIZ, 0, (struct sockaddr *)&(listeClientUDP[i].adresse), (socklen_t *) &lenghtOfAdresse) == -1) {
					perror("erreur recvfrom");
				}
				else {
					printf("Recu : %s\n", request);
					if(answer != NULL) memset(answer, 0, sizeof(answer));
					answer = handlePollingRequest(request);
					if(sendto(listeClientUDP[i].socket, answer, strlen(answer), 0, (struct sockaddr *)&(listeClientUDP[i].adresse), sizeof(listeClientUDP[i].adresse)) == -1) {
						perror("Erreur 2e sendto");
					}
				}
			}
		}


		//Si on recoit une nouvelle connexion, on l'ajoute à la liste des clients
		if(select(descripteursSockets[nbSockets - 1] + 1, &readFds, 0, 0, &temps) < 0) {
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
					sockTCP = accept(descripteursSockets[i], (struct sockaddr *) 0, (unsigned int*) 0);
					listeClientTCP = ajouterClientTCP(listeClientTCP, &nbClientTCP, sockTCP);
				}
				else {
					printf("Requete UDP arrivée sur la socket n°%d\n", i);
					struct sockaddr_in from;
					int lenghtOfFrom;

					lenghtOfFrom = sizeof(from);
					if(recvfrom(descripteursSockets[i], buffer, BUFSIZ, 0, (struct sockaddr *) &from, (socklen_t *) &lenghtOfFrom) < 0) {
						perror("Erreur recvfrom");
					}
					else {
						clUDP.socket = descripteursSockets[i];
						clUDP.adresse = from;
						listeClientUDP = ajouterClientUDP(listeClientUDP, &nbClientUDP, clUDP);
					}
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

	return 0;
}

/***************************
		clientLoop
***************************/
int clientLoop(int protocolType, char* nomDistant, u_short portDistant, int protocolHandlerId) {
	char* (*fonctionGenerateRequest[PROTOCOL_HANDLERS_COUNT])(void) = {generateTestRequest, generateCsmaCDRequest};
	int (*fonctionHandleAnswer[PROTOCOL_HANDLERS_COUNT])(char*) = {handleTestAnswer, handleCsmaCDAnswer};

	int socket;
	struct hostent* hoteDistant;
	struct sockaddr_in adresseDistante;

	if(protocolHandlerId < 0 || protocolHandlerId > PROTOCOL_HANDLERS_COUNT - 1) {
		printf("Identifiant du gestionnaire de protocole incorrect : %d\n", protocolHandlerId);
		return -1;
	}

	if(protocolType == SOCK_DGRAM) {
		socket = creerSocket(portDistant, protocolType, 0);
	}

	if((hoteDistant = gethostbyname(nomDistant)) == NULL) {
		perror("Impossible de résourdre le nom du serveur distant");
		return -1;
	}

	memcpy((char *)&adresseDistante.sin_addr, (char *)hoteDistant->h_addr, hoteDistant->h_length);
	adresseDistante.sin_family = AF_INET;
	adresseDistante.sin_port = htons(portDistant);

	//Boucle principale du client (pour le moment infinie)
	while(1) {
		//Attente d'un temps aléatoire entre [SLEEP_CLIENT_MIN ; SLEEP_CLIENT_MAX[
		sleep(entierAleatoireEntreBorne(SLEEP_CLIENT_MIN, SLEEP_CLIENT_MAX));
		if(protocolType == SOCK_STREAM) {
			//Protocole TCP
			socket = creerSocket(portDistant, protocolType, 0);
			if(envoyerRequeteTCP(&socket, &adresseDistante, fonctionGenerateRequest[protocolHandlerId], fonctionHandleAnswer[protocolHandlerId]) < 0) {
				//Si erreur, on quitte la boucle pour fermer la socket
				perror("Erreur envoyerRequeteTCP");
				break;
			}
			close(socket);
		} else {
			//Protocole UDP
			if(envoyerRequeteUDP(&socket, &adresseDistante, fonctionGenerateRequest[protocolHandlerId], fonctionHandleAnswer[protocolHandlerId]) < 0) {
				//Si erreur, on quitte la boucle pour fermer la socket
				perror("Erreur envoyerRequeteUDP");
				break;
			}
		}
	}
	close(socket);
	return 0;
}


/***************************
		clientPollingLoop
***************************/
int clientPollingLoop(int protocolType, char* nomDistant, u_short portDistant) {
	int socket;
	struct hostent* hoteDistant;
	struct sockaddr_in adresseDistante;
	int lenghtOfAdresseDistante = sizeof(adresseDistante);
	char messageServeur[BUFSIZ];
	char* UDPConnexionMessage = "c";

	socket = creerSocket(portDistant, protocolType, 0);

	if((hoteDistant = gethostbyname(nomDistant)) == NULL) {
		perror("Impossible de résourdre le nom du serveur distant");
		return -1;
	}

	memcpy((char *)&adresseDistante.sin_addr, (char *)hoteDistant->h_addr, hoteDistant->h_length);
	adresseDistante.sin_family = AF_INET;
	adresseDistante.sin_port = htons(portDistant);

	//Connexion au serveur
	if(protocolType == SOCK_STREAM) {
		if (connect(socket, (struct sockaddr *)&adresseDistante, sizeof(adresseDistante)) == -1) {
			perror("Erreur connexion");
			return(-1);
		}
	}
	else {
		//Envois d'un premier message pour initialiser la connexion au serveur
		if (sendto(socket, UDPConnexionMessage, strlen(UDPConnexionMessage), 0, (struct sockaddr *)&adresseDistante, sizeof(adresseDistante)) == -1) {
			perror("Erreur connexion");
			return(-1);
		}
	}

	char* request;
	char answer[BUFSIZ];
	//Boucle principale du client (pour le moment infinie)
	while(1) {
		if(protocolType == SOCK_STREAM) {
			memset(messageServeur, 0, sizeof(messageServeur));
			if(read(socket, messageServeur, BUFSIZ) == -1) {
				perror("Erreur 1e read");
				return -1;
			}
			printf("Message recu : %s\n", messageServeur);
			request = generatePollingRequest();
			if(write(socket, request, strlen(request)) == -1) {
				perror("Erreur write");
				free(request);
				return -1;
			}
			if(read(socket, answer, BUFSIZ) == -1) {
				perror("Erreur 2e read");
				free(request);
				return -1;
			}
			printf("Message recu : %s\n", answer);
		}
		else {
			memset(messageServeur, 0, sizeof(messageServeur));
			if (recvfrom(socket, messageServeur, BUFSIZ, 0, (struct sockaddr *)&adresseDistante, (socklen_t*) &lenghtOfAdresseDistante) == -1) {
				perror("Erreur recvfrom");
				return -1;
			}
			printf("Message recu : %s\n", messageServeur);
			request = generatePollingRequest();
			if (sendto(socket, request, strlen(request), 0, (struct sockaddr *)&adresseDistante, sizeof(adresseDistante)) == -1) {
				perror("Erreur connexion");
				free(request);
				return -1;
			}
			if (recvfrom(socket, answer, BUFSIZ, 0, (struct sockaddr *)&adresseDistante, (socklen_t*) &lenghtOfAdresseDistante) == -1) {
				perror("Erreur recvfrom");
				free(request);
				return -1;
			}
			printf("Message recu : %s\n", answer);
		}
	}
	close(socket);
	return 0;
}

/***************************
		ajouterClientTCP
***************************/
int* ajouterClientTCP(int* tableauClient, int* nombreClient, int client) {
	tableauClient = realloc(tableauClient, *nombreClient+1);
	tableauClient[*nombreClient] = client;
	(*nombreClient)++;
	return tableauClient;
}

/***************************
		retirerClientTCP
***************************/
int* retirerClientTCP(int* tableauClient, int* nombreClient, int client) {
	int i;
	int clientTrouve = 0;
	for(i = 0; i < *nombreClient; i++) {
		if(tableauClient[i] == client) {
			clientTrouve = 1;
		}
		if(clientTrouve == 1) {
			tableauClient[i] = tableauClient[i+1];
		}
	}
	if(clientTrouve == 1) {
		tableauClient = realloc(tableauClient, *nombreClient-1);
		(*nombreClient)--;
	}
	return tableauClient;
}

/***************************
		ajouterClientUDP
***************************/
clientUDP* ajouterClientUDP(clientUDP* tableauClient, int* nombreClient, clientUDP client) {
	tableauClient = realloc(tableauClient, *nombreClient+1);
	tableauClient[*nombreClient] = client;
	(*nombreClient)++;
	return tableauClient;
}

/***************************
		envoyerRequeteUDP
***************************/
int envoyerRequeteUDP(int* socket, struct sockaddr_in* adresseDistante, char* (fonctionGenerateRequest)(void), int (fonctionHandleAnswer)(char *)) {
	char* request;
	char answer[BUFSIZ];
	int lenghtOfFrom;

	lenghtOfFrom = sizeof(*adresseDistante);

	request = fonctionGenerateRequest();
	if(sendto(*socket, request, strlen(request) + 1, 0, (struct sockaddr *) adresseDistante, (socklen_t) lenghtOfFrom) < 0) {
		perror("Erreur sendto");
		free(request);
		return -1;
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		printf("Ecriture réussie\n");
=======
		printf("Ecriture réussie : %s\n", request);
>>>>>>> upstream/master
=======
		printf("Ecriture réussie : %s\n", request);
>>>>>>> upstream/master
		if(recvfrom(*socket, answer, BUFSIZ, 0, (struct sockaddr *) adresseDistante, (socklen_t *) &lenghtOfFrom) < 0) {
			perror("Erreur recvfrom");
			free(request);
			return -1;
		} else  {
			printf("Message reçu : %s\n",answer);
			if(fonctionHandleAnswer(answer) < 0) {
				perror("Erreur handleAnswer");
				free(request);
				return -1;
			}
		}
	}
	free(request);
	return 0;
}

/***************************
		envoyerRequeteTCP
***************************/
int envoyerRequeteTCP(int* socket, struct sockaddr_in* adresseDistante, char* (fonctionGenerateRequest)(void), int (fonctionHandleAnswer)(char *)) {
	char* request;
	char answer[BUFSIZ];

	request = fonctionGenerateRequest();

	if (connect(*socket, (struct sockaddr *) adresseDistante, sizeof(*adresseDistante)) == -1) {
		perror("Connexion à l'hôte distant impossible");
		free(request);
		return -1;
	}

	if(write(*socket,request,strlen(request) + 1) < 0) {
		perror("Erreur write");
		free(request);
		return -1;
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		printf("Ecriture réussie\n");
=======
		printf("Ecriture réussie : %s\n", request);
>>>>>>> upstream/master
=======
		printf("Ecriture réussie : %s\n", request);
>>>>>>> upstream/master
		if(read(*socket, answer, BUFSIZ) < 0) {
			perror("Erreur read");
			free(request);
			return -1;
		} else  {
			printf("Message reçu : %s\n",answer);
			if(fonctionHandleAnswer(answer) < 0) {
				perror("Erreur handleAnswer");
				free(request);
				return -1;
			}
		}
	}
	free(request);
	return 0;
}

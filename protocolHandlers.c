#include "protocolHandlers.h"

//Pointeur generique vers la memoire partagee
static void* sharedDatas = NULL;
static int maxLigne = -1;

//Structure permettant de stocker les donnees partagees entre les processus pour le protocole Test
typedef struct {
    int requestNumber;
} testSharedDatas;

typedef struct {
    int inUse;
} csmaCDSharedDatas;

/***************************
		testSharedInitializer
***************************/
int testSharedInitializer(void) {
    sharedDatas = (testSharedDatas*) malloc(sizeof(testSharedDatas));

    //Comme le pointeur est de type void*, on spécifie explicitement que c'est un pointeur sur une structure testSharedData
    testSharedDatas* datas = sharedDatas;
    datas->requestNumber = 0;
    return 0;
}

/***************************
		testSharedCleaner
***************************/
int testSharedCleaner(void) {
    free(sharedDatas);
    return 0;
}

/***************************
		handleTestRequest
***************************/
char* handleTestRequest(char* request) {
    char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    testSharedDatas* datas = sharedDatas;
    snprintf(answer, BUFSIZ, "Request %i : '%s' - OK%c", datas->requestNumber, request, '\0');
    datas->requestNumber++;

    return answer;
}

/***************************
		generateTestRequest
***************************/
char* generateTestRequest() {
    time_t now;
    time(&now);
    char* request = (char*) malloc(BUFSIZ * sizeof(char));
    snprintf(request, BUFSIZ,"echo from %d at %s",getpid(), ctime(&now));
    request[strlen(request) - 1] = '\0';
    return request;
}

/***************************
		handleTestAnswer
***************************/
int handleTestAnswer(char * answer) {
    printf("Message reçu du protocole de test : %s\n", answer);
    return 0;
}

/***************************
		csmaCDSharedInitializer
***************************/
int csmaCDSharedInitializer(void) {
    sharedDatas = (csmaCDSharedDatas*) malloc(sizeof(csmaCDSharedDatas));

    //Comme le pointeur est de type void*, on spécifie explicitement que c'est un pointeur sur une structure testSharedData
    csmaCDSharedDatas* datas = sharedDatas;
    datas->inUse = 0;
    return 0;
}

/***************************
		csmaCDSharedCleaner
***************************/
int csmaCDSharedCleaner(void) {
    csmaCDSharedDatas* datas = sharedDatas;
    free(datas);
    return 0;
}

/***************************
		handleCsmaCDRequest
***************************/
char* handleCsmaCDRequest(char* request) {
<<<<<<< HEAD
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));
    csmaCDSharedDatas* datas = sharedDatas;
    FILE* ressource = NULL;
    trame* donnees = extractDatas(request);
    char* token = strtok(donnees->fonction, ",");

    if(datas->inUse) {
        donnees->code = 'E';
        snprintf(answerFonction, BUFSIZ, "SB,%c", '\0');
    } else {
        datas->inUse = 1;
        if(OuvrirEnAppend(&ressource, FILE_NAME) < 0) {
            perror("Problème lors de l'ouverture du fichier");
        }
        if(strcmp(token,"w") == 0) {
            donnees->code = 'R';
            token = strtok(NULL, ",");
            if(EcrireLigne(ressource, token) < 0) {
                perror("Problème lors de l'écriture dans le fichier");
            }
            snprintf(answerFonction, BUFSIZ, "wrtOk,%c", '\0');
        } else if (strcmp(token,"cnt") == 0){
            donnees->code = 'R';
            int nbLignes = NombreLigne(ressource);
            snprintf(answerFonction, BUFSIZ, "cntOk,%d,%c", nbLignes, '\0');
        } else if (strcmp(token, "rd") == 0) {
            int numLigne = atoi(strtok(NULL, ","));
            char* buffer = (char *) malloc(BUFSIZ * sizeof(char));
            if(LireLigne(ressource, numLigne, buffer) < 0) {
                donnees->code = 'E';
                snprintf(answerFonction, BUFSIZ, "IOOB,%c", '\0');
            } else {
                donnees->code = 'R';
                snprintf(answerFonction, BUFSIZ, "rdOk,%s,%c", buffer, '\0');
            }
            free(buffer);
        } else {
            donnees->code = 'E';
            snprintf(answerFonction, BUFSIZ, "UF,%c", '\0');
        }
        if(FermerFichier(ressource) < 0) {
            perror("Problème lors de la fermeture du fichier");
        }
        datas->inUse = 0;
    }

    free(donnees->fonction);
    donnees->fonction = answerFonction;

    char* answer = writeTrame(donnees);
    free(donnees->fonction);
    free(donnees);

    return answer;
}


/***************************
		generateCsmaCDRequest
***************************/
char* generateCsmaCDRequest() {
    time_t now;
    trame datas;
    datas.type = 'Q';
    datas.code = 'F';
    datas.fonction = (char*) malloc(BUFSIZ * sizeof(char));
    time(&now);

    int i = entierAleatoireEntreBorne(0,3);
    switch(i) {
        case 0 : {
            snprintf(datas.fonction, BUFSIZ, "w,Le processus %d a ecrit a %s,%c", getpid(), ctime(&now), '\0');
            break;
        }

        case 1 : {
            if(maxLigne > -1) {
                snprintf(datas.fonction, BUFSIZ, "rd,%d,%c", entierAleatoireEntreBorne(0,maxLigne), '\0');
                break;
            }
        }

        default : {
            snprintf(datas.fonction, BUFSIZ, "cnt,%c", '\0');
            break;
        }
    }
    char* request = writeTrame(&datas);
    free(datas.fonction);
    return request;
}

/***************************
		handleCsmaCDAnswer
***************************/
int handleCsmaCDAnswer(char * answer) {
    trame* donnees = extractDatas(answer);

    char* token = strtok(donnees->fonction, ",");
    if(donnees->code = 'R') {
        if(strcmp(token,"rdOk") == 0) {
            token = strtok(NULL, ",");
            printf("Ligne lue : '%s'\n", token);
        } else if(strcmp(token,"wrtOk") == 0) {
            printf("Ecriture dans le fichier réussie\n");
        } else if(strcmp(token,"cntOk") == 0) {
            token = strtok(NULL, ",");
            printf("Nombre de lignes du fichier : %s\n", token);
            maxLigne = atoi(token);
        } else {
            printf("Impossible d'interpréter le résultat\n");
        }
    } else if(donnees->code = 'E') {
        if(strcmp(token, "SB") == 0) {
            int attente = entierAleatoireEntreBorne(0, MAX_ATTENTE_CSMA);
            printf("Serveur occupé, attente de %d secondes...\n", attente);
            sleep(attente);
            printf("Reprise...\n");
        } else {
            printf("Erreur, code erreur = %s\n", token);
        }
    } else {
        printf("Code non reconnue\n");
    }
    free(donnees->fonction);
    free(donnees);
    return 0;
}

/***************************
        generatePollingRequest
***************************/
char* generatePollingRequest() {
    time_t now;
    char type = 'Q';
    char code = 'F';
    char* fonction = (char*) malloc(BUFSIZ * sizeof(char));
    char* request = (char*) malloc(BUFSIZ * sizeof(char));
    time(&now);

    int i = entierAleatoireEntreBorne(0,3);
    switch(i) {
        case 0 : {
            snprintf(fonction, BUFSIZ, "w,Le processus %d a ecrit a %s%c", getpid(), ctime(&now), '\0');
            break;
        }

        case 1 : {
            snprintf(fonction, BUFSIZ, "cnt,%c", '\0');
            break;
        }

        default : {
            snprintf(fonction, BUFSIZ, "rd,%d%c", entierAleatoireEntreBorne(0,100), '\0');
            break;
        }
    }

    snprintf(request, BUFSIZ, "%c%c%03d%s%c", type, code, strlen(fonction), fonction, '\0');

    free(fonction);
    return request;
}

/***************************
        handlePollingRequest
***************************/
char* handlePollingRequest(char* request) {
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));
    FILE* ressource = NULL;
    trame* donnees = extractDatas(request);

    char* token = strtok(donnees->fonction, ",");

    if(OuvrirEnAppend(&ressource, "ressource") < 0) {
        perror("Problème lors de l'ouverture du fichier");
    }
    if(strcmp(token,"w") == 0) {
        donnees->code = 'R';
        token = strtok(NULL, ",");
        if(EcrireLigne(ressource, token) < 0) {
            perror("Problème lors de l'écriture dans le fichier");
        }
        snprintf(answerFonction, BUFSIZ, "wrtOk,%c", '\0');
    } else if (strcmp(token,"cnt") == 0){
        donnees->code = 'R';
        int nbLignes = NombreLigne(ressource);
        snprintf(answerFonction, BUFSIZ, "cntOk,%d,%c", nbLignes, '\0');
    } else if (strcmp(token, "rd") == 0) {
        int numLigne = atoi(strtok(NULL, ","));
        char* buffer = (char *) malloc(BUFSIZ * sizeof(char));
        if(LireLigne(ressource, numLigne, buffer) < 0) {
            donnees->code = 'E';
            snprintf(answerFonction, BUFSIZ, "IOOB,%c", '\0');
        } else {
            donnees->code = 'R';
            snprintf(answerFonction, BUFSIZ, "rdOk,%s,%c", buffer, '\0');
        }
        free(buffer);
    } else {
        donnees->code = 'E';
        snprintf(answerFonction, BUFSIZ, "UF,%c", '\0');
    }
    if(FermerFichier(ressource) < 0) {
        perror("Problème lors de la fermeture du fichier");
    }

    free(donnees->fonction);
    donnees->fonction = answerFonction;

    char* answer = writeTrame(donnees);
    free(donnees->fonction);
    free(donnees);

    return answer;

    /*char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));

    char type = request[0];
    char code = request[1];
    char* idStr = extractSubstring(request, 2, 12);
    char* tailleStr = extractSubstring(request, 3, 15);

    int id = atoi(idStr);
    int taille = atoi(tailleStr);
    char* fonction = extractSubstring(request, 15,15 + taille);


    char* token = strtok(fonction, ",");

    if(strcmp(token,"w") == 0) {
        snprintf(answerFonction, BUFSIZ, "wASKED%c", '\0');
    } else if (strcmp(token,"cnt") == 0){
        snprintf(answerFonction, BUFSIZ, "cntASKED%c", '\0');
    } else if (strcmp(token, "rd") == 0) {
        snprintf(answerFonction, BUFSIZ, "rdASKED%c", '\0');
    } else {
        code = 'E';
        snprintf(answerFonction, BUFSIZ, "UF%c", '\0');
    }

    snprintf(answer, BUFSIZ, "%c%c%010d%03d%s%c", type, code, id, strlen(answerFonction), answerFonction, '\0');
    free(answerFonction);
    free(idStr);
    free(tailleStr);
    free(fonction);
=======
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));
    csmaCDSharedDatas* datas = sharedDatas;
    FILE* ressource = NULL;
    trame* donnees = extractDatas(request);
    char* token = strtok(donnees->fonction, ",");

    if(datas->inUse) {
        donnees->code = 'E';
        snprintf(answerFonction, BUFSIZ, "SB,%c", '\0');
    } else {
        datas->inUse = 1;
        if(OuvrirEnAppend(&ressource, FILE_NAME) < 0) {
            perror("Problème lors de l'ouverture du fichier");
        }
        if(strcmp(token,"w") == 0) {
            donnees->code = 'R';
            token = strtok(NULL, ",");
            if(EcrireLigne(ressource, token) < 0) {
                perror("Problème lors de l'écriture dans le fichier");
            }
            snprintf(answerFonction, BUFSIZ, "wrtOk,%c", '\0');
        } else if (strcmp(token,"cnt") == 0){
            donnees->code = 'R';
            int nbLignes = NombreLigne(ressource);
            snprintf(answerFonction, BUFSIZ, "cntOk,%d,%c", nbLignes, '\0');
        } else if (strcmp(token, "rd") == 0) {
            int numLigne = atoi(strtok(NULL, ","));
            char* buffer = (char *) malloc(BUFSIZ * sizeof(char));
            if(LireLigne(ressource, numLigne, buffer) < 0) {
                donnees->code = 'E';
                snprintf(answerFonction, BUFSIZ, "IOOB,%c", '\0');
            } else {
                donnees->code = 'R';
                snprintf(answerFonction, BUFSIZ, "rdOk,%s,%c", buffer, '\0');
            }
            free(buffer);
        } else {
            donnees->code = 'E';
            snprintf(answerFonction, BUFSIZ, "UF,%c", '\0');
        }
        if(FermerFichier(ressource) < 0) {
            perror("Problème lors de la fermeture du fichier");
        }
        datas->inUse = 0;
    }

    free(donnees->fonction);
    donnees->fonction = answerFonction;

    char* answer = writeTrame(donnees);
    free(donnees->fonction);
    free(donnees);
>>>>>>> upstream/master

    return answer;*/
}


/***************************
        handlePollingAnswer
***************************/
int handlePollingAnswer(char * answer) {
    printf("Protocole Polling : réponse = %s\n", answer);
    return 0;
}

/***************************
        generatePollingRequest
***************************/
char* generatePollingRequest() {
    time_t now;
    trame datas;
    datas.type = 'Q';
    datas.code = 'F';
    datas.fonction = (char*) malloc(BUFSIZ * sizeof(char));
    time(&now);

    int i = entierAleatoireEntreBorne(0,3);
    switch(i) {
        case 0 : {
            snprintf(datas.fonction, BUFSIZ, "w,Le processus %d a ecrit a %s,%c", getpid(), ctime(&now), '\0');
            break;
        }

        case 1 : {
            if(maxLigne > -1) {
                snprintf(datas.fonction, BUFSIZ, "rd,%d,%c", entierAleatoireEntreBorne(0,maxLigne), '\0');
                break;
            }
        }

        default : {
            snprintf(datas.fonction, BUFSIZ, "cnt,%c", '\0');
            break;
        }
    }
    char* request = writeTrame(&datas);
    free(datas.fonction);
    return request;
}

/***************************
		handleCsmaCDAnswer
***************************/
int handleCsmaCDAnswer(char * answer) {
    trame* donnees = extractDatas(answer);

    char* token = strtok(donnees->fonction, ",");
    if(donnees->code = 'R') {
        if(strcmp(token,"rdOk") == 0) {
            token = strtok(NULL, ",");
            printf("Ligne lue : '%s'\n", token);
        } else if(strcmp(token,"wrtOk") == 0) {
            printf("Ecriture dans le fichier réussie\n");
        } else if(strcmp(token,"cntOk") == 0) {
            token = strtok(NULL, ",");
            printf("Nombre de lignes du fichier : %s\n", token);
            maxLigne = atoi(token);
        } else {
            printf("Impossible d'interpréter le résultat\n");
        }
    } else if(donnees->code = 'E') {
        if(strcmp(token, "SB") == 0) {
            int attente = entierAleatoireEntreBorne(0, MAX_ATTENTE_CSMA);
            printf("Serveur occupé, attente de %d secondes...\n", attente);
            sleep(attente);
            printf("Reprise...\n");
        } else {
            printf("Erreur, code erreur = %s\n", token);
        }
    } else {
        printf("Code non reconnue\n");
    }
    free(donnees->fonction);
    free(donnees);
    return 0;
}

/***************************
        generatePollingRequest
***************************/
char* generatePollingRequest() {
    time_t now;
    char type = 'Q';
    char code = 'F';
    char* fonction = (char*) malloc(BUFSIZ * sizeof(char));
    char* request = (char*) malloc(BUFSIZ * sizeof(char));
    time(&now);

    int i = entierAleatoireEntreBorne(0,3);
    switch(i) {
        case 0 : {
            snprintf(fonction, BUFSIZ, "w,Le processus %d a ecrit a %s%c", getpid(), ctime(&now), '\0');
            break;
        }

        case 1 : {
            snprintf(fonction, BUFSIZ, "cnt,%c", '\0');
            break;
        }

        default : {
            snprintf(fonction, BUFSIZ, "rd,%d%c", entierAleatoireEntreBorne(0,100), '\0');
            break;
        }
    }

    snprintf(request, BUFSIZ, "%c%c%03d%s%c", type, code, strlen(fonction), fonction, '\0');

    free(fonction);
    return request;
}

/***************************
        handlePollingRequest
***************************/
char* handlePollingRequest(char* request) {
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));
    FILE* ressource = NULL;
    trame* donnees = extractDatas(request);

    char* token = strtok(donnees->fonction, ",");

    if(OuvrirEnAppend(&ressource, "ressource") < 0) {
        perror("Problème lors de l'ouverture du fichier");
    }
    if(strcmp(token,"w") == 0) {
        donnees->code = 'R';
        token = strtok(NULL, ",");
        if(EcrireLigne(ressource, token) < 0) {
            perror("Problème lors de l'écriture dans le fichier");
        }
        snprintf(answerFonction, BUFSIZ, "wrtOk,%c", '\0');
    } else if (strcmp(token,"cnt") == 0){
        donnees->code = 'R';
        int nbLignes = NombreLigne(ressource);
        snprintf(answerFonction, BUFSIZ, "cntOk,%d,%c", nbLignes, '\0');
    } else if (strcmp(token, "rd") == 0) {
        int numLigne = atoi(strtok(NULL, ","));
        char* buffer = (char *) malloc(BUFSIZ * sizeof(char));
        if(LireLigne(ressource, numLigne, buffer) < 0) {
            donnees->code = 'E';
            snprintf(answerFonction, BUFSIZ, "IOOB,%c", '\0');
        } else {
            donnees->code = 'R';
            snprintf(answerFonction, BUFSIZ, "rdOk,%s,%c", buffer, '\0');
        }
        free(buffer);
    } else {
        donnees->code = 'E';
        snprintf(answerFonction, BUFSIZ, "UF,%c", '\0');
    }
    if(FermerFichier(ressource) < 0) {
        perror("Problème lors de la fermeture du fichier");
    }

    free(donnees->fonction);
    donnees->fonction = answerFonction;

    char* answer = writeTrame(donnees);
    free(donnees->fonction);
    free(donnees);

    return answer;

    /*char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));

    char type = request[0];
    char code = request[1];
    char* idStr = extractSubstring(request, 2, 12);
    char* tailleStr = extractSubstring(request, 3, 15);

    int id = atoi(idStr);
    int taille = atoi(tailleStr);
    char* fonction = extractSubstring(request, 15,15 + taille);


    char* token = strtok(fonction, ",");

    if(strcmp(token,"w") == 0) {
        snprintf(answerFonction, BUFSIZ, "wASKED%c", '\0');
    } else if (strcmp(token,"cnt") == 0){
        snprintf(answerFonction, BUFSIZ, "cntASKED%c", '\0');
    } else if (strcmp(token, "rd") == 0) {
        snprintf(answerFonction, BUFSIZ, "rdASKED%c", '\0');
    } else {
        code = 'E';
        snprintf(answerFonction, BUFSIZ, "UF%c", '\0');
    }

    snprintf(answer, BUFSIZ, "%c%c%010d%03d%s%c", type, code, id, strlen(answerFonction), answerFonction, '\0');
    free(answerFonction);
    free(idStr);
    free(tailleStr);
    free(fonction);

    return answer;*/
}

/***************************
        handlePollingAnswer
***************************/
int handlePollingAnswer(char * answer) {
    printf("Protocole Polling : réponse = %s\n", answer);
    return 0;
}

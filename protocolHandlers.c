#include "protocolHandlers.h"

//Pointeur generique vers la memoire partagee
static void* sharedDatas = NULL;

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
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));
    csmaCDSharedDatas* datas = sharedDatas;
    FILE* ressource = NULL;
    trame* donnees = extractDatas(request);

    char* token = strtok(donnees->fonction, ",");

    if(datas->inUse) {
        donnees->code = 'E';
        snprintf(answerFonction, BUFSIZ, "SB%c", '\0');
    } else {
        datas->inUse = 1;
        if(OuvrirEnAppend(&ressource, "ressource") < 0) {
            perror("Problème lors de l'ouverture du fichier");
        }
        if(strcmp(token,"w") == 0) {
            donnees->code = 'R';
            token = strtok(NULL, ",");
            if(EcrireLigne(ressource, token) < 0) {
                perror("Problème lors de l'écriture dans le fichier");
            }
            snprintf(answerFonction, BUFSIZ, "ok,%c", '\0');
        } else if (strcmp(token,"cnt") == 0){
            donnees->code = 'R';
            snprintf(answerFonction, BUFSIZ, "rdCNT,%c",'\0');
        } else if (strcmp(token, "rd") == 0) {
            snprintf(answerFonction, BUFSIZ, "rdASKED%c", '\0');
        } else {
            donnees->code = 'E';
            snprintf(answerFonction, BUFSIZ, "UF%c", '\0');
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
    datas.id = 0;
    datas.fonction = (char*) malloc(BUFSIZ * sizeof(char));
    time(&now);

    int i = entierAleatoireEntreBorne(0,3);
    switch(i) {
        case 0 : {
            snprintf(datas.fonction, BUFSIZ, "w,Le processus %d a ecrit a %s,%c", getpid(), ctime(&now), '\0');
            break;
        }

        case 1 : {
            snprintf(datas.fonction, BUFSIZ, "cnt,%c", '\0');
            break;
        }

        default : {
            snprintf(datas.fonction, BUFSIZ, "rd,%d,%c", entierAleatoireEntreBorne(0,100), '\0');
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
    printf("Protocole CSMA : réponse = %s\n", answer);
    return 0;
}

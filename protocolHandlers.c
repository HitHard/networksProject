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
    free(sharedDatas);
    return 0;
}

/***************************
		handleCsmaCDRequest
***************************/
char* handleCsmaCDRequest(char* request) {
    char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    char* answerFonction = (char*) malloc(BUFSIZ * sizeof(char));

    char type = request[0];
    char code = request[1];
    char* idStr = extractSubstring(request, 2, 12);
    char* tailleStr = extractSubstring(request, 3, 15);

    int id = atoi(idStr);
    int taille = atoi(tailleStr);
    char* fonction = extractSubstring(request, 15,15 + taille);

    csmaCDSharedDatas* datas = sharedDatas;

    char* token = strtok(fonction, ",");

    if(datas->inUse) {
        code = 'E';
        snprintf(answerFonction, BUFSIZ, "SB%c", '\0');
    } else {
        datas->inUse = 1;
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
        datas->inUse = 0;
    }
    snprintf(answer, BUFSIZ, "%c%c%010d%03d%s%c", type, code, id, strlen(answerFonction), answerFonction, '\0');

    free(answerFonction);
    free(idStr);
    free(tailleStr);
    free(fonction);

    return answer;
}


/***************************
		generateCsmaCDRequest
***************************/
char* generateCsmaCDRequest() {
    time_t now;
    char type = 'Q';
    char code = 'F';
    int id = 0;
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

    snprintf(request, BUFSIZ, "%c%c%010d%03d%s%c", type, code, id, strlen(fonction), fonction, '\0');

    free(fonction);
    return request;
}

/***************************
		handleCsmaCDAnswer
***************************/
int handleCsmaCDAnswer(char * answer) {
    printf("Protocole CSMA : réponse = %s\n", answer);
    return 0;
}

/***************************
        generatePollingRequest
***************************/
char* generatePollingRequest() {
    time_t now;
    char type = 'Q';
    char code = 'F';
    int id = 0;
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

    snprintf(request, BUFSIZ, "%c%c%010d%03d%s%c", type, code, id, strlen(fonction), fonction, '\0');

    free(fonction);
    return request;
}

/***************************
        handlePollingRequest
***************************/
char* handlePollingRequest(char* request) {
    char* answer = (char*) malloc(BUFSIZ * sizeof(char));
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

    return answer;
}

/***************************
        handlePollingAnswer
***************************/
int handlePollingAnswer(char * answer) {
    printf("Protocole Polling : réponse = %s\n", answer);
    return 0;
}
#include "protocolHandlers.h"

//Pointeur generique vers la memoire partagee
static void* sharedDatas = NULL;

//Structure permettant de stocker les donnees partagees entre les processus pour le protocole Test
typedef struct {
    int requestNumber;
    pthread_mutex_t mutex;
} testSharedDatas;

/***************************
		testSharedInitializer
***************************/
int testSharedInitializer(void) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;
    if((sharedDatas = mmap(NULL, sizeof(testSharedDatas), prot, flags, -1, 0)) == MAP_FAILED) {
        perror("Impossible de placer la structure en mémoire partagée");
        return -1;
    }

    //Comme le pointeur est de type void*, on spécifie explicitement que c'est un pointeur sur une structure testSharedData
    testSharedDatas* datas = sharedDatas;
    datas->requestNumber = 0;
    pthread_mutexattr_t attr;
    if(pthread_mutexattr_init(&attr) != 0) {
        perror("Erreur lors de l'initialisation de l'attribut mutex");
        return -1;
    }
    if(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        perror("Erreur lors de l'initialisation de la valeur 'process-shared' de l'attribut mutex");
        return -1;
    }
    if(pthread_mutex_init(&datas->mutex, &attr) != 0) {
        perror("Erreur lors de l'initialisation du mutex");
        return -1;
    }
    return 0;
}

/***************************
		testSharedCleaner
***************************/
int testSharedCleaner(void) {
    if(munmap(sharedDatas, sizeof(testSharedDatas)) == -1) {
        perror("Impossible de nettoyer la mémoire partagée");
        return -1;
    }
    return 0;
}

/***************************
		handleTestRequest
***************************/
char* handleTestRequest(char* request) {
    char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    testSharedDatas* datas = sharedDatas;
    pthread_mutex_lock(&datas->mutex);
    snprintf(answer, BUFSIZ, "Request %i : '%s' - OK%c", datas->requestNumber, request, '\0');
    datas->requestNumber++;
    pthread_mutex_unlock(&datas->mutex);

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

#include "protocolHandlers.h"

/***************************
		handleTestRequest
***************************/
char* handleTestRequest(char* request) {
    char* answer = (char*) malloc(BUFSIZ * sizeof(char));
    strcpy(answer, request);
    answer[strlen(request) - 1] = '\0';
    strcat(answer, " - OK\0");
    return answer;
}

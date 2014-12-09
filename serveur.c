#include "networkFunctions.h"

int main(int argc, char* argv[]) {
	//arguments : nomProgramme(0) protocoleSession(1)
    if(argc < 2) {
        perror("Protocole Session non specifié (0->Test, 1->CSMA, 2->Demande, 3->Polling)");
        exit(-1);
    }

    int protocoleSession = atoi(argv[1]);

    if(protocoleSession < 3) {
    	serverLoop(3,5,12332,protocoleSession);
    }
    else {
    	serverPollingLoop(3,5,12332);
    }

    return 0;
}

all: serveur client

serveur: serveur.o fileFunctions.o networkFunctions.o protocolHandlers.o
	gcc -o serveur serveur.o fileFunctions.o networkFunctions.o protocolHandlers.o -lpthread

client: client.o fileFunctions.o networkFunctions.o protocolHandlers.o
	gcc -o client client.o fileFunctions.o networkFunctions.o protocolHandlers.o -lpthread

fileFunctions.o: fileFunctions.c fileFunctions.h
	gcc -c fileFunctions.c -Wall -Wextra

networkFunctions.o: networkFunctions.c networkFunctions.h
	gcc -c networkFunctions.c -Wall -Wextra

protocolHandlers.o: protocolHandlers.c protocolHandlers.h
	gcc -c protocolHandlers.c -lpthread -Wall -Wextra

serveur.o: serveur.c
	gcc -c serveur.c -Wall -Wextra

client.o: client.c
	gcc -c client.c -Wall -Wextra

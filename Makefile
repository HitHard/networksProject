all: serveur client

serveur: tools.o protocolHandlers.o fileFunctions.o networkFunctions.o serveur.o
	gcc -o serveur serveur.o fileFunctions.o networkFunctions.o protocolHandlers.o tools.o -lpthread

client: tools.o protocolHandlers.o fileFunctions.o networkFunctions.o client.o
	gcc -o client client.o fileFunctions.o networkFunctions.o protocolHandlers.o tools.o -lpthread

fileFunctions.o: fileFunctions.c fileFunctions.h
	gcc -c fileFunctions.c -Wall -Wextra

networkFunctions.o: networkFunctions.c networkFunctions.h
	gcc -c networkFunctions.c -Wall -Wextra

protocolHandlers.o: protocolHandlers.c protocolHandlers.h
	gcc -c protocolHandlers.c -lpthread -Wall -Wextra

tools.o: tools.c tools.h
	gcc -c tools.c -Wall -Wextra

serveur.o: serveur.c
	gcc -c serveur.c -Wall -Wextra

client.o: client.c
	gcc -c client.c -Wall -Wextra

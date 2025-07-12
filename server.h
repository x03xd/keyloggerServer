#ifndef SERVER_H
#define SERVER_H

#define TOTAL_BUFFER_SIZE 4800
#define USER_ID_SIZE 37
#define RESULT_SIZE 3000

typedef struct ThreadArgs {
    SOCKET acceptSocket;
} ThreadArgs;


int handleConnection(LPVOID args);

void createAndSaveToTheFile(char userId[], char result[]);

void parseBinaryMessage(char buffer[], char userId[], char result[]);

#endif

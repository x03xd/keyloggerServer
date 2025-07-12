#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include "include/thread-args.h"


BOOL initializeServer(SOCKET serverSocket, int socketResult, BOOL wsaInitialized, WSADATA wsaData, unsigned short port);
BOOL acceptClientsLoop(SOCKET serverSocket, SOCKET clientSocket, HANDLE thread, ThreadArgs* args, char mode[]);

#endif

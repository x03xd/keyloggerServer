#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include "server.h" 


int main() {
    WSADATA wsaData;
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    int socketResult = 0;
    BOOL wsaInitialized = FALSE;
    HANDLE thread = NULL;
    ThreadArgs* args = NULL;

    socketResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (socketResult != 0) {
        goto CLEANUP;
    }
    wsaInitialized = TRUE;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        goto CLEANUP;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4321);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        goto CLEANUP;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        goto CLEANUP;
    }

    while (1) {
        clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        if (!args) {
            goto CLEANUP;
        }
        args->acceptSocket = clientSocket;

        thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleConnection, args, 0, NULL);
        if (thread == NULL) {
            goto CLEANUP;
        }
        
        WaitForSingleObject(thread, INFINITE);
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

void parseBinaryMessage(char buffer[], char userId[], char result[]) {
    DWORD resultLen = 0;
    DWORD userIdLen = 0;
    memcpy(&resultLen, buffer, sizeof(DWORD));
    memcpy(&userIdLen, buffer + sizeof(DWORD), sizeof(DWORD));
    memcpy(result, buffer + 2 * sizeof(DWORD), resultLen);
    memcpy(userId, buffer + 2 * sizeof(DWORD) + resultLen, userIdLen);
}

int handleConnection(LPVOID args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    SOCKET acceptSocket = threadArgs->acceptSocket;
    int bytesReceived = 0;
    char buffer[TOTAL_BUFFER_SIZE];
    char userId[USER_ID_SIZE];
    char result[RESULT_SIZE];

    while (1) {
        bytesReceived = recv(acceptSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == SOCKET_ERROR && bytesReceived == 0) {
            break;
        }
        buffer[bytesReceived] = '\0';
        parseBinaryMessage(buffer, userId, result);
        createAndSaveToTheFile(userId, result);
    }
    
    return 1;
}

void createAndSaveToTheFile(char userId[], char result[]) {
    FILE *file = fopen(userId, "a");
    if (!file) return;

    size_t len = strlen(result);
    if (fwrite(result, 1, len, file) != len) {
        fclose(file);
        return;
    }

    fclose(file);
}

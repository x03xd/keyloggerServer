#include <windows.h>
#include <stdio.h>
#include "payload-server.h" 
#include "include/thread-args.h"


int handlePayload(LPVOID args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    SOCKET acceptSocket = threadArgs->acceptSocket;
    int bytesReceived = 0;

    while (1) {

    }

    return 1;
}

#ifndef SERVER_H
#define SERVER_H

typedef struct Config {
    unsigned short port;
    char mode[16];
} Config;


BOOL parseArgs(int argc, char* argv[], Config* config);

#endif

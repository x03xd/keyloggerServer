#ifndef LOGGING_SERVER_H
#define LOGGING_SERVER_H

#define TOTAL_BUFFER_SIZE 4800
#define USER_ID_SIZE 37
#define RESULT_SIZE 3000


int handleLogging(LPVOID args);

void parseBinaryMessage(char buffer[], char userId[], char result[]);

void createAndSaveToTheFile(char userId[], char result[]);

#endif

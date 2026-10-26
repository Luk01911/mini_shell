#include "shell.h"
#include <string.h>

int parse_line(char *line, char **args) {
    int i = 0;

    // Remove the trailing newline character left by fgets
    line[strcspn(line, "\n")] = 0;

    // Tokenize the string by spaces, tabs, and carriage returns
    char *token = strtok(line, " \t\r\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL; // CRITICAL: execvp requires a NULL-terminated array

    return i;
}

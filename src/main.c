#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    int status = 1;

    // Main REPL Loop
    do {
        // 1. Print Prompt
        printf("mini_shell> ");
        fflush(stdout); // Ensure the prompt prints immediately

        // 2. Read Input
        if (fgets(line, sizeof(line), stdin) == NULL) {
            // Handle EOF (Ctrl+D) gracefully
            printf("\n");
            break;
        }

        // 3. Parse Line into Tokens
        parse_line(line, args);

        // 4. Execute Command & Check Status (0 means exit)
        status = execute_command(args);

    } while (status);

    return EXIT_SUCCESS;
}

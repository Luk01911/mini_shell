#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int execute_command(char **args) {
    // 1. Check for empty command (user just hit Enter)
    if (args[0] == NULL) {
        return 1;
    }

    // 2. Handle Built-in: exit
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Signals main loop to terminate
    }

    // 3. Handle Built-in: cd (Change Directory)
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "mini_shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("mini_shell");
            }
        }
        return 1; // Keep shell running
    }

    // 4. Handle External Commands (Fork, Exec, Wait)
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("mini_shell: fork failed");
    } else if (pid == 0) {
        // --- CHILD PROCESS ---
        // Overwrite child image with external binary
        if (execvp(args[0], args) == -1) {
            perror("mini_shell");
        }
        exit(EXIT_FAILURE); // If execvp fails, exit child immediately
    } else {
        // --- PARENT PROCESS ---
        // Wait for the child process to finish executing
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1; // Keep shell running
}

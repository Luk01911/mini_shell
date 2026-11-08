#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h> // Required for open(), O_WRONLY, O_RDONLY, O_CREAT, O_TRUNC

int execute_command(char **args) {
    // 1. Check if the command line was empty
    if (args[0] == NULL) {
        return 1;
    }

    // 2. Handle built-in "exit" command
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    // 3. Handle built-in "cd" (change directory) command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "mini_shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("mini_shell");
            }
        }
        return 1;
    }

    // 4. Check for a pipe ("|") in the command arguments
    int pipe_idx = -1;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_idx = i;
            break;
        }
    }

    if (pipe_idx != -1) {
        // --- PIPE EXECUTION PATH ---
        args[pipe_idx] = NULL; // Split arguments into left and right commands
        char **args1 = args;
        char **args2 = &args[pipe_idx + 1];

        if (args1[0] == NULL || args2[0] == NULL) {
            fprintf(stderr, "mini_shell: syntax error near unexpected token '|'\n");
            return 1;
        }

        int pipefd[2];
        if (pipe(pipefd) < 0) {
            perror("mini_shell: pipe failed");
            return 1;
        }

        // Fork Child 1 (Left command)
        pid_t pid1 = fork();
        if (pid1 < 0) {
            perror("mini_shell: fork failed");
            return 1;
        }

        if (pid1 == 0) {
            // Redirect stdout to write end of the pipe
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);

            if (execvp(args1[0], args1) == -1) {
                perror("mini_shell");
            }
            exit(EXIT_FAILURE);
        }

        // Fork Child 2 (Right command)
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("mini_shell: fork failed");
            return 1;
        }

        if (pid2 == 0) {
            // Redirect stdin to read end of the pipe
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);

            if (execvp(args2[0], args2) == -1) {
                perror("mini_shell");
            }
            exit(EXIT_FAILURE);
        }

        // Parent process closes both pipe ends
        close(pipefd[0]);
        close(pipefd[1]);

        // Wait for both children to finish
        int status;
        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);

        return 1;
    }

    // 5. Standard single-command execution path (with optional < or > redirection)
    pid_t pid = fork();

    if (pid < 0) {
        perror("mini_shell: fork failed");
    } else if (pid == 0) {
        // ==========================================
        // --- CHILD PROCESS ---
        // ==========================================
        
        int i = 0;
        int fd;
        while (args[i] != NULL) {
            // Check for output redirection (">")
            if (strcmp(args[i], ">") == 0) {
                if (args[i + 1] == NULL) {
                    fprintf(stderr, "mini_shell: syntax error near unexpected token 'newline'\n");
                    exit(EXIT_FAILURE);
                }

                fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("mini_shell: open failed");
                    exit(EXIT_FAILURE);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[i] = NULL;
                break;
            }
            // Check for input redirection ("<")
            else if (strcmp(args[i], "<") == 0) {
                if (args[i + 1] == NULL) {
                    fprintf(stderr, "mini_shell: syntax error near unexpected token 'newline'\n");
                    exit(EXIT_FAILURE);
                }

                fd = open(args[i + 1], O_RDONLY);
                if (fd < 0) {
                    perror("mini_shell: open failed");
                    exit(EXIT_FAILURE);
                }

                dup2(fd, STDIN_FILENO);
                close(fd);
                args[i] = NULL;
                break;
            }
            i++;
        }

        if (execvp(args[0], args) == -1) {
            perror("mini_shell");
        }
        exit(EXIT_FAILURE);
    } else {
        // ==========================================
        // --- PARENT PROCESS ---
        // ==========================================
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

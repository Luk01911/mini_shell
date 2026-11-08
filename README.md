# Mini Shell (`mini_shell`)

A lightweight, modular POSIX-compliant Unix command shell written from scratch in **C99**. Built to master low-level operating system architecture, process lifecycle management, file descriptor manipulation, and systems programming fundamentals on Linux.

## Features

- **Modular Architecture:** Clean separation of concerns with isolated modules for the REPL loop (`main`), string tokenization (`parser`), and process execution (`executor`).
- **Process Management:** Uses core POSIX system calls (`fork`, `execvp`, `waitpid`) to safely spawn, execute, and reap external child processes without leaving zombie processes.
- **Built-In Commands:** 
  - `cd [path]`: Dynamically changes the working directory using system-level `chdir()`.
  - `exit`: Gracefully terminates the shell REPL loop.
- **I/O Redirection:** Supports output redirection (`>`) and input redirection (`<`) using low-level file descriptor duplication (`dup2`, `open`).
- **Pipes (`|`):** Supports single-pipe command chaining (`cmd1 | cmd2`) via inter-process communication, dual child process orchestration, and strict file descriptor cleanup to prevent deadlocks.
- **Robust Input Handling:** Cleans trailing newlines (`strcspn`) and handles `EOF` (`Ctrl+D`) gracefully.

## Project Structure

mini_shell/
├── include/
│   └── shell.h         # Global constants, macros, and function prototypes
├── src/
│   ├── main.c          # REPL loop and entry point
│   ├── parser.c        # Command-line string tokenizer
│   └── executor.c      # Built-in handler, redirection, and pipe/process engine
├── Makefile            # Build automation configuration
└── README.md
Getting Started
Prerequisites
A Linux environment (tested on Arch Linux)

gcc compiler supporting C99

make build utility

Building and Running
Clone the repository:

Bash
git clone [https://github.com/Luk01911/mini_shell.git](https://github.com/Luk01911/mini_shell.git)
cd mini_shell
Compile the project using the Makefile:

Bash
make
Run the shell:

Bash
./mini_shell
Usage Examples
Plaintext
mini_shell> pwd
/home/luis/mini_shell

mini_shell> ls -l
total 24
-rw-r--r-- 1 luis luis  1070 Oct 26 18:00 Makefile
drwxr-xr-x 2 luis luis  4096 Oct 26 18:00 include
drwxr-xr-x 2 luis luis  4096 Oct 26 18:00 src

mini_shell> ls -l | grep src
drwxr-xr-x 2 luis luis  4096 Oct 26 18:00 src

mini_shell> cat files.txt
main.c
parser.c
executor.c

mini_shell> cat < files.txt | grep executor
executor.c

mini_shell> cd include
mini_shell> pwd
/home/luis/mini_shell/include

mini_shell> exit
License
This project is open-source and available under the MIT License.

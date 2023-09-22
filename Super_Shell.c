#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void display_prompt() {
    printf("\033[1;32m");  // Set text color to green
    system("pwd");
    printf("\033[0m");    // Reset text color
    printf("> ");
    fflush(stdout);
}

int main() {
    char input[512];
    char *command;
    char *args[10];

    system("clear");
    while(1) {
        display_prompt();

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        // Remove trailing newline character
        input[strcspn(input, "\n")] = 0;

        // Tokenize the input
        command = strtok(input, " ");
        int i = 0;
        while (command != NULL) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Check for built-in commands
        if (strcmp(args[0], "cd") == 0) {
            if (chdir(args[1]) != 0) {
                perror("chdir");
            }
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        }

        // Fork a new process
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child process
            if (strcmp(args[0], "ls") == 0) {
                execl("/bin/ls", "ls", NULL);
            } else if (strcmp(args[0], "processes") == 0) {
                execl("/bin/ps", "ps", "-e", NULL);
            } else if (strcmp(args[0], "vi") == 0) {
                execl("/usr/bin/vi", "vi", args[1], NULL);
            } else if (strcmp(args[0], "view") == 0) {
                execl("/bin/cat", "cat", args[1], NULL);
            } else if (strcmp(args[0], "rm") == 0) {
                execl("/bin/rm", "rm", args[1], NULL);
            } else if (strcmp(args[0], "scan-suid") == 0) {
                system("find / -type f -perm -4000 -exec ls -la {} 2>/dev/null \;");
            } else {
                printf("Unknown command: %s\n", args[0]);
                exit(1);
            }
        } else {
            // Parent process
            wait(NULL);
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

pid_t foreground_pid = -1;

/* -------- Ctrl-C handler -------- */
void sigint_handler(int sig) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
    } else {
        printf("\nmysh > ");
        fflush(stdout);
    }
}

/* -------- parse -------- */
int parse(char *line, char **argv) {
    int argc = 0;
    char *tok = strtok(line, " \t\n");

    while (tok && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }

    argv[argc] = NULL;
    return argc;
}

/* -------- PIPE HANDLER -------- */
int handle_pipe(char **argv) {
    int i = 0;

    while (argv[i] != NULL && strcmp(argv[i], "|") != 0)
        i++;

    if (argv[i] == NULL) return 0;

    argv[i] = NULL;
    char **cmd1 = argv;
    char **cmd2 = &argv[i + 1];

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(cmd1[0], cmd1);
        perror("execvp");
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(cmd2[0], cmd2);
        perror("execvp");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return 1;
}

/* -------- REDIRECTION -------- */
int handle_redirect(char **argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], ">") == 0) {

            argv[i] = NULL;
            char *file = argv[i + 1];

            if (fork() == 0) {
                int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);

                execvp(argv[0], argv);
                perror("execvp");
                exit(1);
            }

            wait(NULL);
            return 1;
        }
    }
    return 0;
}

/* -------- MAIN -------- */
int main(void) {
    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    signal(SIGINT, sigint_handler);

    while (1) {
        printf("mysh > ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        int argc = parse(line, argv);
        if (argc == 0) continue;

        /* EXIT */
        if (strcmp(argv[0], "exit") == 0)
            break;

        /* CD (built-in) */
        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1])
                chdir(argv[1]);
            continue;
        }

        /* BACKGROUND */
        int background = 0;
        if (strcmp(argv[argc - 1], "&") == 0) {
            background = 1;
            argv[argc - 1] = NULL;
        }

        /* PIPE */
        if (handle_pipe(argv))
            continue;

        /* REDIRECTION */
        if (handle_redirect(argv))
            continue;

        pid_t pid = fork();

        if (pid == 0) {
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {

            if (!background) {
                foreground_pid = pid;
                waitpid(pid, NULL, 0);
                foreground_pid = -1;
            } else {
                printf("[background pid %d]\n", pid);
            }

        } else {
            perror("fork");
        }
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    for (int i = 0; i < 3; i++) {

        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child index: %d, PID: %d\n", i, getpid());

            return 0;
        }
    }

    // Parent waits for all children
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    printf("All done\n");

    return 0;
}

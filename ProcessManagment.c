#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main (void) {

    //variable declaration
    const int numberOfChildren = 15;
    pid_t childPids[numberOfChildren];
    int status;
    int codeZeroStatus = 0;
    int terminatedStatus = 0;
    int nonZeroStatus = 0;

    //commands array to hold 15 unique Linux commands
    char *commands[][3] = {
        {"echo", "Hello Calvin Giles", NULL },
        {"date", NULL, NULL},
        {"ls", "-l", NULL},
        {"invalid_cmd1", NULL, NULL},
        {"pwd", NULL, NULL},
        {"mkdir", "src", NULL},
        {"wc", "file.txt", NULL},
        {"touch", "file1.txt", NULL},
        {"stat", "file1.txt", NULL},
        {"cat", "renamedFile.txt", NULL},
        {"rm", "renamedFile.txt", NULL},
        {"uname", NULL, NULL},
        {"invalid_cmd2", NULL, NULL},
        {"uptime", NULL, NULL},
        {"whoami", NULL, NULL}
    };

    printf("\nParent process PID: %d\n", getpid());

    //for loop create the children with fork method
    //basic error handling with pid<0
    //aborts 2 commands 
    for (int i = 0; i < numberOfChildren; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            //error 
            perror("Fork Failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            //child process
            printf("[Child #%d] PID: %d - about to run: %s\n",i, getpid(), commands[i][0]);
            
            //make two children crash
            if (i == 6 || i == 8) {
                fprintf(stderr, "[Child #%d] Intentionally calling abort() to crash.\n", i);
                abort(); //SIGABRT
            }
            
            execvp(commands[i][0], commands[i]);

            perror("Exe failed");

            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            //parent process(child PID)
            printf("[Parent] create Child #%d with PID=%d\n", i, pid);
            childPids[i] = pid;
        }
    }

    //parenent waits in creation order
    printf("\n=== Parent waiting in CREATION ORDER with exit-status reporting ===\n");

    for (int i = 0; i < numberOfChildren; i++) {
        pid_t w = waitpid(childPids[i], &status, 0);

        //error
        if (w < 0) {
            perror("waitpid failed");
            continue;
        }

        printf("[Parent] Child #%d PID=%d finished: ", i, w);

        if (WIFEXITED(status)) {
            printf("exited normally, exit_code=%d\n", WEXITSTATUS(status));
            
            if (WEXITSTATUS(status) == 0) {
                codeZeroStatus++;  
            } else {
                nonZeroStatus++;
            }
            
        } else if (WIFSIGNALED(status)) {
            printf("terminated by signal %d\n", WTERMSIG(status));
            terminatedStatus++;
        } 
    }

    //displays a summary of processes created and executed
    printf("\n=== Summary ===\n");
    printf("Total children: 15\n");
    printf("Exited with code 0 (success): %d\n", codeZeroStatus);
    printf("Exited with non-zero code:    %d\n", nonZeroStatus);
    printf("Terminated by signal:         %d\n", terminatedStatus);

    return EXIT_SUCCESS;
}

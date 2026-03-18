#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>


#define MAX_LINE 1024  
#define MAX_TOKENS 64

typedef struct {
    char *argv[MAX_TOKENS];
    char *out_file;
} Command;

int parse_line(char *line, Command *cmd) {
    
    memset(cmd, 0, sizeof(Command));
    int argc = 0;
    char token[MAX_LINE];
    int ti = 0;
    int i = 0;
    int len = strlen(line);

    //process each char in input line
    while (i < len) {

        //skip whitespace 
        while (i<len && isspace((unsigned char)line[i])) {
            i++;
        }//end inner while loop
        if (i>=len) {break;}
    
        //read files 
        ti = 0;

        if (line[i] == '>') {
            //skip '>'
            i++;

            //skip space
            while (i < len && !isspace((unsigned char)line[i]))
                i++;
            //read file names
            char filebuf[MAX_LINE];
            int fi = 0;

            while (i < len && !isspace((unsigned char)line[i]))
                filebuf[fi++] = line[i++];
            //store the file name
            filebuf[fi] = 0;
            cmd->out_file = strdup(filebuf);
            continue;

        }

        //read until whitespace or '>' is found
        while (i < len && !isspace((unsigned char)line[i]) && line[i] != '>') {
            token[ti++] = line[i++];
        }   

        token[ti] = 0;

        //add token to argv if not empty
        if (ti > 0) {
            cmd->argv[argc++] = strdup(token);
        }
    }//end of outer while loop

    //null terminate the argv array
    cmd->argv[argc] = NULL;

    return argc;
}

void execute_command(Command *cmd){
    pid_t pid = fork();

    if(pid < 0) {
        perror("Fork error");
        return;
    }

    if (pid == 0) {

        for (int i = 0; cmd->argv[i]; i++) {
            if (strncmp(cmd->argv[i], "__IN__", 6) == 0) {
                char *file = cmd->argv[i] + 6;
                int fd = open(file, O_RDONLY);
                if (fd < 0) {
                    perror("error");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                free(cmd->argv[i]);

                for (int j = i; cmd->argv[j]; j++) {
                    cmd->argv[j];
                }
                i--;
            }
        }

        if (cmd->out_file) {
            int flags = O_WRONLY | O_CREAT;
            char *fname = cmd->out_file;

            if (strncmp(fname, ">>", 2) == 0) {
                //append
                flags |= O_APPEND;
                fname += 2;
            } else {
                flags |= O_TRUNC;
            }

            int fd = open(cmd->out_file, flags, 0644);
            if (fd < 0) {
                perror("open >");
                exit(1);
            }
            //redirect stdout to file
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(cmd->argv[0], cmd->argv);
        perror("Execvp");
        exit(1);
    }

    //wait till child is complete
    waitpid(pid, NULL, 0);
}

//check if the cmd is built in 
int is_builtin(char *cmd) {
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0);
}

//exe built in cmds
int run_builtin(Command *cmd) {

    if (strcmp(cmd->argv[0], "cd") == 0) {
        char *dir = cmd->argv[1];
        if (!dir)
            dir = getenv("HOME");
        if (chdir(dir) != 0)
            perror("cd");
        return 0; 
    }

    if (strcmp(cmd->argv[0], "exit") == 0) {
        return 1;
    }

    return 0;

}

int main() {

    char line[MAX_LINE];

    while (1) { 

        printf("myshell> ");
        fflush(stdout);   

        if (!fgets(line, sizeof(line), stdin))
            break;  
        
        line[strcspn(line, "\n")] = 0;

        Command cmd;
        if (parse_line(line, &cmd) == 0)
            continue;
        if (cmd.argv[0] == NULL)
            continue;
        if (is_builtin(cmd.argv[0])) {

            if (run_builtin(&cmd))
                break;
        } else {
            execute_command(&cmd);
        }

    }

    
    return 0;
}
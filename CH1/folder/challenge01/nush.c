#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

void
execute(char* cmd)
{
    int cpid;

    if ((cpid = fork())) {
        // parent process
        printf("Parent pid: %d\n", getpid());
        printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

        printf("== executed program complete ==\n");

        printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }
    }
    else {
        // child process
        printf("Child pid: %d\n", getpid());
        printf("Child knows parent pid: %d\n", getppid());

        for (int ii = 0; ii < strlen(cmd); ++ii) {
            if (cmd[ii] == ' ') {
                cmd[ii] = 0;
                break;
            }
        }

        // The argv array for the child.
        // Terminated by a null pointer.
        char* args[] = {cmd, "one", 0};

        printf("== executed program's output: ==\n");

        execvp(cmd, args);
        printf("Can't get here, exec only returns on error.");
    }
}

int
main(int argc, char* argv[])
{
    char array[100];
    char *space;
    char invert[100][100];
    char *hold;
    char *other;
	
    char cmd[256];

    if (argc == 1) {
        printf("nush$ ");
        fflush(stdout);
        fgets(cmd, 256, stdin);
    }
    else {
        memcpy(cmd, "echo", 5);
    }

    while((fgets(array, sizeof array, stdin)) != NULL){
        array[strcspn(array, "\n")] = 0;
        space = array + strlen(array);
        while(isspace(*--space));
        *(space+1) = '\0';

        char *tok = array, *end = array;
        int i = 0, j = 0;

    while (tok != NULL){
            strsep(&end, " ");
            hold = tok;
            while((other = strsep(&hold, ";")) != NULL){
                if(j > 0){
                    strcpy(invert[i+1], ";");
                    strcpy(invert[i+2], other);
                    i = i + 2;
                }
                else if(j == 0){
                    strcpy(invert[i], other);
                }
                j++;
            }
            j = 0;
            tok = end;
            i++;
        }
        i--;
    }


    execute(cmd);

    return 0;
}

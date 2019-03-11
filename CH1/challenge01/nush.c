#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

void
execute(int arg1, char* args[], char* file, int file_flag, int flag ){
    int cpid;
    int keep[2];

    if(arg1 < 1){
	return;
    }

    char* item = args[0];

    if(strcmp(item, "exit") == 0){
	exit(0);
    }

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
        
	switch(flag){
	    case 20:
                keep[0] = open(file, O_RDONLY);
		dup2(keep[0], STDIN_FILENO);
		close(keep[0]);
		break;
            case 10:
		keep[0] = open(file, O_WRONLY | O_CREAT, 0644);
		dup2(keep[0], STDOUT_FILENO);
		close(keep[0]);
		break;
	}

        execvp(item, args);
        printf("Can't get here, exec only returns on error.");
    }
}

void breakdown(char* item){
    
    const char* delim = " \n";
    char* tok;

    tok = strtok(item, delim);
    char* args[256];
    args[0] = tok;

    int i = 0;
    while(tok != NULL){
	tok = strtok(NULL, delim);
	args[++i] = tok;
    }

    bool match = false;
    int flag = 0;

    for (int j = 1; j < i; j++){
	int z = j + 1;
	if (strcmp(args[j], "<") == 0 && z < i){
	    match = true;

	    char* cmd[j + 1];
	    reduce(item, args, 0, j);
            execute(j, item, args[z], 20, flag);
	    break;
	}    
    	else if (strcmp(args[j], ">") == 0 && z < i){
	    match = true;

	    char* item[j + 1];
	    reduce(item, args, 0, j);
	    execute(j, item, args[z], 10, flag);
	    break;
	}
	else if (strcmp(args[j], "&") == 0){
	    flag = 1;
	}
	else if (strcmp(args[j], "&&") == 0){
	    match = true;
	    if (strcmp(args[0], "false") == 0){
		break;
            }
	    else {
                char* prev[j + 1];
		reduce(prev, args, 0, j);

		char* bull = "";
		execute(j, prev, bull, 0, flag);

		int post = i - z;
		char* next[post + 1];
		reduce(next, args, z, i);
		execute(post, next, bull, 0, flag);
	    }
	    break;
	}
	else if(strcmp(args[j], "||") == 0){
            match = true;
	    if(strcmp(args[0], "false") == 0){
		int post = i -z;
		char* next[post + 1];
		reduce(next, args, z, i);
                char* bull = "";
		execute(post, next, bull, 0, flag);
	    }
	    else{
		char* prev[j + 1];
		reduce(prev, args, 0, j);
		char* bull = "";
		execute(j, prev, bull, 0, flag);
	    }
	    break;
	}
    }

    if(!match){
	char* bull = "";
	execute(i, args, bull, 0, flag);
    }
}

void reduce(char* dest[], char* orig[], int begin, int end){
    if(begin >= end){
	printf("Beginning parameter cannot be greater that ending\n");
	return;
    }
    int last = end - begin;
    for(int i = 0; i < last; i++){
	dest[i] = orig[begin + i];
    }

    dest[last] = NULL;
}

int
main(int argc, char* argv[])
{
    char array[100];
    char *space;
    char invert[100][100];
    char *hold;
    char *other;
	
    FILE* send;
	
    char cmd[256];

    if (argc == 1) {
        printf("nush$ ");
        fflush(stdout);
        send = stdin;
        //fgets(cmd, 256, stdin);
    }
    else {
        //memcpy(cmd, "echo", 5);
	send = fopen(argv[1], "r");
    }

    while((fgets(array, sizeof array, send)) != NULL){
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
        for (int z = 0; z < i; z++){
	    breakdown(array[z]);
        }

        if (argc == 1){
            printf("nush$ ");
        }	
    }	
    return 0;
}

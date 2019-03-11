//Written by Andrew Ricci

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int _ac, char* _av[]){
    char array[100];
    char *space;
    char invert[100][100];
    char *hold;
    char *other; 

    printf("tokens$ ");
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
	for(int j = 0; j <= i; i--){
	    printf("%s\n", invert[i]);
	}

	if(_ac == 1){
	    printf("tokens$ ");
	}
    }
}

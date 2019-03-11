//Written by Andrew Ricci

#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int floorSqrt(int x){
    if(x == 0 || x == 1){
	return x;
    }
    int i = 1, result = 1;
    while(result <= x){
	i ++;
        result = i * i;
    }
    return i - 1;
}

int
main(int _ac, char* _av[])
{
    int memory[1200];
    int a[100][100];
    int i,n,lines;
    for(i = 0; i < 1200; i++){
        if(fscanf(stdin, "%d", (memory+i)) == EOF){
	    break;
	}
    }
    lines = floorSqrt(i);
    int z = 0;
    for(int row = 0; row < lines; row++){
	for(int col = 0; col < lines; col++){
            a[row][col] = memory[z];
            z++;
	}
    }
    
    for(int j = 0; j < lines; j++){
	for(int k = 0; k < lines; k++){
	    printf("%d ", a[k][j]);
	}
	printf("\n");
    }

    return 0;
}


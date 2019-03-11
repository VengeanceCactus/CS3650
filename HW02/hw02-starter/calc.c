//Written by Andrew Ricci

#include "stdio.h"
#include "stdlib.h"

long calc(long x, char op, long y){
    if(op == '+'){
        return x + y;
    }
    else if(op == '-'){
	return x - y;
    }
    else if(op == '*'){
	return x * y;
    }
    else if(op == '/'){
	return x / y;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 4){
        printf("Usage: please use format ./ccalc n op m");
	return 1;
    }
    if(*argv[2] == '%' && (atol(argv[1]) < atol(argv[3]))){
	printf("Usage: please don't do this");
	return 1;
    }
    long value = calc(atol(argv[1]), *argv[2], atol(argv[3]));
    printf("%ld %s %ld %s %ld", atol(argv[1]), argv[2], atol(argv[3]), "=", value);
}

    

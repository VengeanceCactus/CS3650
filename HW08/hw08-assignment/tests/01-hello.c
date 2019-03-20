
#include <stdio.h>
#include <string.h>

#include "hmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    char* hello = hstrdup("Hello, World");
    printf("%s\n", hello);
    hfree(hello);

    hprintstats();
    return 0;
}




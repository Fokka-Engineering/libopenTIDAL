#include <stdio.h>
#include <stdlib.h>

typedef struct test {
    char *string;
} test;

void
allocHelper (test **ptr)
{
    *ptr = malloc (sizeof (test));
}

int
main ()
{
    test *ptr;

    allocHelper (&ptr);
    ptr->string = "String Content";
    printf ("String: %s\n", ptr->string);
}

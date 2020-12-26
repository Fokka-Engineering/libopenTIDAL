#include <stdio.h>
#include <stdlib.h>

#include "../src/include/handles.h"
#include "../src/include/openTIDAL.h"

int main()
{
    openTIDAL_SessionContainer session;
    openTIDAL_CurlContainer curl;

    openTIDAL_Verbose(3);
    openTIDAL_SessionInit(&session, NULL);
    openTIDAL_CurlRequest(&session, &curl, "GET", "/v1/search",
            "countryCode=DE&query=Sting", NULL, 0, 0);

    printf("Pointer: %p\n",curl.body);
    printf("Stuff: %s\n", curl.body);

    free(curl.body);
}

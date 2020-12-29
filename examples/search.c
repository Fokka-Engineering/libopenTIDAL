#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int i = 0;

int main()
{
    openTIDAL_Verbose(3);
    openTIDAL_SessionContainer session;
    openTIDAL_SessionInit(&session, NULL);

    openTIDAL_ContentContainer result = openTIDAL_SearchAll(&session, "Tingvall Trio", 10);
    if (result.status == 1)
    {
        for (i = 0; i < result.total[0]; ++i)
        {
            printf("Album #%d\n", i);
            printf("Title: %s\n", result.albums[i].title);
            printf("Copyright: %s\n", result.albums[i].copyright);
            printf("\n");
        }
        for (i = 0; i < result.total[2]; ++i)
        {
            printf("Artist #%d\n", i);
            printf("Name: %s\n", result.artists[i].name);
            printf("Id: %zu\n", result.artists[i].id);
            printf("Picture: %s\n", result.artists[i].picture);
        }
    }
    
    openTIDAL_StructDelete(&result);
    openTIDAL_SessionCleanup(&session);
    return 0;
}

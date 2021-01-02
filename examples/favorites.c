#include <stdio.h>
#include <stdlib.h>

#include "../src/openTIDAL.h"

int
main ()
{
    openTIDAL_SessionContainer session;
    int status;
    openTIDAL_Verbose (3);

    openTIDAL_SessionInit (&session, "/Users/hugo/Documents/oT-config");

    const char *ids[2] = { "157173297", "159643944" };

    status = openTIDAL_AddFavoriteAlbums (&session, ids, 2, "FAIL");
    // status = openTIDAL_AddFavoriteVideo (&session, "165770063", "FAIL");
    // status = openTIDAL_DeleteFavoriteVideo (&session, "165770063");

    openTIDAL_SessionCleanup (&session);

    return 0;
}

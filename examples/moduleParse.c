#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int
main ()
{
    openTIDAL_SessionContainer session;

    openTIDAL_Verbose (1);
    openTIDAL_SessionInit (&session, "/Users/hugo/Documents/oT-config");

    openTIDAL_ContentContainer *resolve = openTIDAL_GetHome (&session);

    printf ("ArraySize: %d\n", resolve->modules.arraySize);
    printf ("Type #2: %d\n", resolve->modules.arrayType[2]);
    printf ("Name #2: %s\n", resolve->modules.moduleType[2]);
    printf ("Start #2: %d\n", resolve->modules.offset[2]); // Not Workin
    printf ("Total: %d\n", resolve->modules.total[2]);
    openTIDAL_StructDelete (resolve);
    openTIDAL_SessionCleanup (&session);
    return 0;
}

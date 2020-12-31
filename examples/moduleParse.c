#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int
main ()
{
    openTIDAL_SessionContainer session;
    int i;

    openTIDAL_Verbose (1);
    openTIDAL_SessionInit (&session, "/Users/hugo/Documents/oT-config");

    openTIDAL_ContentContainer *resolve = openTIDAL_GetHome (&session);

    printf ("ArraySize: %d\n", resolve->modules.arraySize);
    for (i = 0; i < resolve->modules.arraySize; i++) {
        printf ("Type : %d\n", resolve->modules.arrayType[i]);
        printf ("TypeString : %s\n", resolve->modules.moduleType[i]);
        printf ("Title : %s\n", resolve->modules.moduleTitle[i]);
        printf ("PreTitle : %s\n", resolve->modules.modulePreTitle[i]);
        printf ("Start : %d\n", resolve->modules.offset[i]);
        printf ("Total: %d\n", resolve->modules.total[i]);
    }
    openTIDAL_StructDelete (resolve);
    openTIDAL_SessionCleanup (&session);
    return 0;
}

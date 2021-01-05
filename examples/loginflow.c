#include "../src/openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
    /* specify a file location (config will be created automatically) */
    openTIDAL_SessionContainer session;
    openTIDAL_Verbose (1);
    openTIDAL_SessionInit (&session, "/Users/hugo/Desktop/config.json");
    openTIDAL_ContentContainer *resolve = openTIDAL_AuthCreateUserCode (&session);
    if (resolve)
        if (resolve->status == 1) {
            printf ("UserCode: %s\n", resolve->code->userCode);
            while (time (NULL) <= resolve->code->expires_in) {
                sleep (resolve->code->interval);
                openTIDAL_ContentContainer *token;

                token = openTIDAL_AuthCreateBearerToken (&session, resolve->code->deviceCode);
                if (token) {
                    if (token->status == 2)
                        printf ("%s\n", "Authorization Pending...");
                    else if (token->status == 1) {
                        printf ("%s\n", "Authorization Successfull!");
                        openTIDAL_SessionCreateFile (&session);
                        break;
                    }
                    openTIDAL_StructDelete (token);
                }
            }
        };
    openTIDAL_StructDelete (resolve);
    openTIDAL_SessionCleanup (&session);
}

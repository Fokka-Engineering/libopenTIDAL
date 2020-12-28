#include <stdio.h>
#include <stdlib.h>
#include "../src/include/openTIDAL.h"
#include <unistd.h>

void login_polling()
{
    openTIDAL_ContentContainer resolve = openTIDAL_AuthCode();
    if (resolve.status == 1)
    {
        printf("DeviceCode: %s\n", resolve.code.deviceCode);
        printf("UserCode: %s\n", resolve.code.userCode);
        size_t i;
        while (time(NULL) <= resolve.code.expires_in)
        {
            sleep(2);
            openTIDAL_ContentContainer token = openTIDAL_CreateLoginToken(resolve.code.deviceCode);
            if (token.status == 2)
            {
                printf("%s\n", "Authorization Pending...");
            }
            else if (token.status == 1)
            {
                printf("%s\n", "Authorization Successfull!");
                printf("AccessToken: %s\n", token.token.access_token);
                printf("RefreshToken: %s\n", token.token.refresh_token);
                printf("Timestamp: %zu\n", token.token.expires_in);
                printf("Username: %s\n", token.token.username);
                printf("CountryCode: %s\n", token.token.countryCode);
                printf("UserId: %zu\n", token.token.userId);
                break;
            }
            else
            {
                printf("%s\n", "Error! .. Retry\n");
                break;
            }

            openTIDAL_StructDelete(&token);
        }
    }
    else
    {
        fprintf(stderr, "DeviceCode Request failed.\n");
    }
    openTIDAL_StructDelete(&resolve);
}

int main(void)
{
    /* specify a file location (config will be created automatically) */
    openTIDAL_Init("/tmp/oT-config");
    login_polling();
    openTIDAL_Cleanup();
}

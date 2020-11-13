#include <stdio.h>
#include <stdlib.h>
#include "../src/include/openTIDAL.h"
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include <unistd.h>

void login_polling()
{
  login_code_model ress = login_create_code();
  if (ress.status == 1)
  {
    printf("DeviceCode: %s\n", ress.deviceCode);
    printf("UserCode: %s\n", ress.userCode);
    size_t i;
    while (time(NULL) <= ress.expires_in)
    {
      sleep(2);
      login_token_model res = login_create_token(ress.deviceCode);
      if (res.status == 2)
      {
        printf("%s\n", "Authorization Pending...");
      }
      else if (res.status == 1)
      {
        printf("%s\n", "Authorization Successfull!");
        printf("AccessToken: %s\n", res.access_token);
        printf("RefreshToken: %s\n", res.refresh_token);
        printf("Timestamp: %zu\n", res.expires_in);
	printf("Username: %s\n", res.username);
	printf("CountryCode: %s\n", res.countryCode);
	printf("UserId: %zu\n", res.userId);
	break;
      }
      else
      {
        printf("%s\n", "Error! .. Retry\n");
        break;
      }
    }
  }
  else
  {
    fprintf(stderr, "DeviceCode Request failed.\n");
  }
}

int main(void)
{
  init("/Users/admin/Documents/Git/openTIDAL/bin/persistent.json");
  login_polling();
  cleanup();
}

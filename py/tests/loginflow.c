#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"
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
    printf("Timestamp: %zu\n", ress.timestamp);
    int i;
    time_t timestampExpired = ress.timestamp + 300;
    printf("Timestamp Expired: %zu\n", timestampExpired);
    while (time(NULL) <= timestampExpired) 
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
        printf("Timestamp: %zu\n", res.timestamp);
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
  init("persistent.json", "LOSSLESS", "HIGH");
  login_polling();
  printf("%zu\n", userId);
  curl_exit_auth();
}

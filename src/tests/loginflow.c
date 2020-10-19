#include <stdio.h>
#include "include/openTIDAL.h"
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>
#include "include/base64.h"

char *client_id = "8SEZWa4J1NVC5U5Y";
char *client_secret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";
char *audioQuality = "LOSSLESS";
char *videoQuality = "HIGH";
char *countryCode = "DE";
char *access_token;

void login_polling()
{
  login_code_model ress = login_create_code();
  printf("%s\n", ress.deviceCode);
  printf("%s\n", ress.userCode);
  int i;
  for (i = 0; i < 100; i++) {
    sleep(2);
    login_token_model res = login_create_token(ress.deviceCode);
    if (res.status == 0)
    {
      printf("%s\n", "Authorization Pending...");
    }
    else if (res.status == 1)
    {
      printf("%s\n", "Authorization Successfull!");
      printf("access_token: %s\n", res.access_token);
      printf("refresh_token: %s\n", res.refresh_token);
      break;
    }
    else
    {
      printf("%s\n", "Error! .. Retry");
      break;
    }
  }
}

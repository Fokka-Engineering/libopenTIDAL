#include <stdlib.h>
#include <stdio.h>
#include "include/cTidal.h"
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>

char *client_id = "8SEZWa4J1NVC5U5Y";
char *client_secret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";
char *soundQuality = "LOSSLESS";
char *countryCode = "DE";
char *access_token = "eyJhbGciOiJIUzI1NiJ9.eyJ0eXBlIjoibzJfYWNjZXNzIiwidWlkIjoxMDI0ODkyMzYsInNjb3BlIjoid19zdWIgcl91c3Igd191c3IiLCJnVmVyIjowLCJzVmVyIjowLCJjaWQiOjIzMjYsImV4cCI6MTYwMzAwNTA4OSwic2lkIjoiYmFlZTcxODctZjUwNi00ZWRhLWIyOWEtOWRjZjFkNDM0OWMyIn0.e6dtg7c60p1vEmA2uEbiRSbjAKg7A4cFOR06MEnfRW8";
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

int main(void)
{
  /*items_model res = get_playlist_items("c24c3adc-6cb6-486c-8a5e-8c1033363619");
  if (res.status != -1)
  {
    int i;
    int e;
    for (i = 0; i < res.arraySize; i++)
    {
      printf("%s\n", res.albumTitle[i]);
      for (e = 0; e < res.subArraySize[i]; e++)
      {
        printf("%s\n", res.artistName[i][e]);
        printf("%zu\n", res.artistId[i][e]);
      }
    }
  }*/
  //login_polling();
  //get_playlist("10e422fd-1cbf-4672-93cd-b8965580fd21");
  stream_model res = get_track_streamUrl(122122043);
  if (res.status != -1)
  {
    printf("%s\n", "Success!");
    printf("%s\n", res.url);
    printf("%zu\n", res.trackId);
    printf("%s\n", res.codec);
    printf("%s\n", res.soundQuality);
  }
  //curl_exit_auth();
}

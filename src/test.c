#include <stdlib.h>
#include <stdio.h>
#include "include/openTIDAL.h"
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
  items_model res = get_mix_items("001fb1d7459dadda7ff467738f665f");
  //items_model res = get_mix_items("004667a04cff872cdd1b9254924935");
  if (res.status == 1)
  {
    int i;
    int e;
    //printf("%s\n", res.title[1]);
    //printf("%zu\n", res.id[1]);
    for (i = 0; i < res.arraySize; ++i)
    {
      printf("%s\n", res.title[i]);
      printf("%zu\n", res.id[i]);
      printf("%zu\n", res.isVideo[i]);
      for (e = 0; e < res.subArraySize[i]; ++e)
      {
        printf("%zu\n", res.artistId[i][e]);
        printf("%s\n", res.artistName[i][e]);
      }
    }
  }
  /*page_mix_model ress = get_user_mixes();
  if (ress.status == 1)
  {
    size_t i;
    for (i = 0; i < ress.arraySize; ++i)
    {
      printf("%s\n", ress.id[i]);
      printf("%s\n", ress.title[i]);
      printf("%s\n", ress.largeImageUrl[i]);
    }
  }*/
  //login_polling();
  //get_playlist("1edbcbcb-3b5a-4fa1-948c-9b4724dff930");
  /*stream_model res = get_track_streamUrl(122122043);
  if (res.status == 1)
  {
    printf("%s\n", "Success!");
    printf("%s\n", res.url);
    printf("%zu\n", res.trackId);
    printf("%s\n", res.codec);
    printf("%s\n", res.soundQuality);
  }*/

  /*char *eTag = get_playlist_etag("1edbcbcb-3b5a-4fa1-948c-9b4724dff930");
  size_t ress = delete_playlist_item("1edbcbcb-3b5a-4fa1-948c-9b4724dff930", 0, eTag);
  if (ress == 1)
  {
    printf("Success!\n");
  }*/
  curl_exit();
}

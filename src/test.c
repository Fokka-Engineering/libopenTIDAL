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
  search_model se = get_search("Sting Essentials", "50");
  if (se.status == 1)
  {
    size_t i;
    size_t a;
    if (strcmp(se.topHitType, "PLAYLISTS") == 0)
    {
      printf("%s\n", se.topPlaylist.uuid[0]);
      printf("%s\n", se.topPlaylist.title[0]);
    }
    if (strcmp(se.topHitType, "ALBUMS") == 0)
    {
      printf("%zu\n", se.topAlbum.id[0]);
      printf("%s\n", se.topAlbum.title[0]);
      printf("%zu\n", se.topAlbum.artistId[0][0]);
      printf("%s\n", se.topAlbum.artistName[0][0]);
    }
    if (strcmp(se.topHitType, "TRACKS") == 0)
    {
      printf("%zu\n", se.topTrack.id[0]);
      printf("%s\n", se.topTrack.title[0]);
      printf("%zu\n", se.topTrack.artistId[0][0]);
      printf("%s\n", se.topTrack.artistName[0][0]);
    }
    
    for (i = 0; i < se.videos.arraySize; ++i)
    {
      printf("%zu\n", se.videos.id[i]);
      printf("%s\n", se.videos.title[i]);
      printf("%s\n", se.videos.cover[i]);
      for (a = 0; a < se.videos.subArraySize[i]; ++a)
      {
        printf("%zu\n", se.videos.artistId[i][a]);
        printf("%s\n", se.videos.artistName[i][a]);
      }
    }
  }
  curl_exit();
}

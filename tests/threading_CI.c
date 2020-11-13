#include <stdlib.h>
#include <stdio.h>
#include "../include/openTIDAL.h"
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>
#include "../include/base64.h"

char *client_id = "8SEZWa4J1NVC5U5Y";
char *client_secret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";

int main(void)
{
  size_t skipped;
  size_t i;
  init("persistent.json");
  printf("Testing openTIDAL Track Endpoints...\n");
  
  /* get_track  */
  skipped = 1;
  printf("[CI] Testing get_track (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    for (i = 0; i < 10; ++i)
    {
      items_model res = get_track(122122043);
        if (res.status == 1)
        {
          int i;
          printf("Id: %zu\n", res.id[0]);
          printf("Title: %s\n", res.title[0]);
          printf("Duration: %zu\n", res.duration[0]);
          printf("Popularity: %zu\n", res.popularity[0]);
          printf("TrackNumber: %zu\n", res.trackNumber[0]);
          printf("VolumeNumber: %zu\n", res.volumeNumber[0]);
          printf("Quality: %s\n", res.quality[0]);
          printf("Cover: %s\n", res.cover[0]);
          printf("AlbumId: %zu\n", res.albumId[0]);
          printf("AlbumTitle: %s\n", res.albumTitle[0]);
          if (res.hasVersion[0] == 1)
          {
            printf("Version: %s\n", res.version[0]);
          }
          for (i = 0; i < res.subArraySize[0]; ++i)
          {
            printf("ArtistId: %zu\n", res.artistId[0][i]);
            printf("ArtistName %s\n", res.artistName[0][i]);
          }
        }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
}

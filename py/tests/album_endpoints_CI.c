#include <stdlib.h>
#include <stdio.h>
#include "../include/openTIDAL.h"
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>
#include "../include/base64.h"

char *client_id = "8SEZWa4J1NVC5U5Y";
char *client_secret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";
char *audioQuality = "LOSSLESS";
char *videoQuality = "HIGH";
char *countryCode = "DE";
char *access_token;

int main(void)
{
  size_t skipped;
  char input_token[1024];

  printf("Testing openTIDAL Album Endpoints...\n");
  printf("[CI] Enter your access_token: ");
  scanf("%s", input_token); /* Pls no BufferOverflow :( */
  access_token = input_token;
  
  /* get_album  */
  skipped = 1;
  printf("[CI] Testing get_album (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    album_model res = get_album(4123207);
    if (res.status == 1)
    {
      int i;
      printf("Id: %zu\n", res.id[0]);
      printf("Title: %s\n", res.title[0]);
      printf("Duration: %zu\n", res.duration[0]);
      printf("Popularity: %zu\n", res.popularity[0]);
      printf("NumberOfTracks: %zu\n", res.numberOfTracks[0]);
      printf("NumberOfVideos: %zu\n", res.numberOfVideos[0]);
      printf("NumberOfVolumes: %zu\n", res.numberOfVolumes[0]);
      printf("ReleaseDate: %s\n", res.releaseDate[0]);
      printf("Copyright: %s\n", res.copyright[0]);
      printf("Quality: %s\n", res.quality[0]);
      printf("Cover: %s\n", res.cover[0]);
      for (i = 0; i < res.subArraySize[0]; ++i)
      {
        printf("ArtistId: %zu\n", res.artistId[0][i]);
        printf("ArtistName %s\n", res.artistName[0][i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_album_items  */
  skipped = 1;
  printf("[CI] Testing get_album_items (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;
    
    printf("Limit: ");
    scanf("%zu", &limit);
    printf("Offset: ");
    scanf("%zu", &offset);

    items_model res = get_album_items(4123207, limit, offset);
    if (res.status == 1)
    {
      int i;
      int a;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("Duration: %zu\n", res.duration[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("TrackNumber: %zu\n", res.trackNumber[i]);
        printf("VolumeNumber: %zu\n", res.volumeNumber[i]);
        if (res.hasVersion[i] == 1)
        {
          printf("Version: %s\n", res.version[i]);
        }
        printf("ReleaseDate: %s\n", res.releaseDate[i]);
        printf("Quality: %s\n", res.quality[i]);
        printf("Cover: %s\n", res.cover[i]);
        if (res.isVideo[i] != 1)
        {
          printf("AlbumId: %zu\n", res.albumId[i]);
   	  printf("AlbumTitle: %s\n", res.albumTitle[i]);
        }
        for (a = 0; a < res.subArraySize[i]; ++a)
        {
          printf("ArtistId: %zu\n", res.artistId[i][a]);
          printf("ArtistName %s\n", res.artistName[i][a]);
        }
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

}

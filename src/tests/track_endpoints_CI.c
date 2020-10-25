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
  init("persistent.json", "LOSSLESS", "HIGH");
  printf("Testing openTIDAL Track Endpoints...\n");
  
  /* get_track  */
  skipped = 1;
  printf("[CI] Testing get_track (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
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
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_track_contributors  */
  skipped = 1;
  printf("[CI] Testing get_track_contributors (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    contributor_model res = get_track_contributors(122122043, limit, offset);
    if (res.status == 1)
    {
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      printf("TotalNumberOfItems: %zu\n", res.totalNumberOfItems);
      int i;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Name: %s\n", res.name[i]);
        printf("Role %s\n", res.role[i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_track_mix  */
  skipped = 1;
  printf("[CI] Testing get_track_mix (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    mix_model res = get_track_mix(122122043);
    if (res.status == 1)
    {
      printf("Id: %s\n", res.id);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_track_stream  */
  skipped = 1;
  printf("[CI] Testing get_track_stream (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    stream_model res = get_track_stream(122122043);
    if (res.status == 1)
    {
      printf("Url: %s\n", res.url);
      printf("TrackId: %zu\n", res.trackId);
      printf("AudioMode: %s\n", res.audioMode);
      printf("AssetPresentation: %s\n", res.assetPresentation);
      printf("audioQuality: %s\n", res.audioQuality);
      printf("MimeType: %s\n", res.mimeType);
      printf("Codec: %s\n", res.codec);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
}

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

  printf("Testing openTIDAL Video Endpoints...\n");
  printf("[CI] Enter your access_token: ");
  scanf("%s", input_token); /* Pls no BufferOverflow :( */
  access_token = input_token;
  
  /* get_video  */
  skipped = 1;
  printf("[CI] Testing get_video (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    items_model res = get_video(58782326);
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

  /* get_video_contributors  */
  skipped = 1;
  printf("[CI] Testing get_video_contributors (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    contributor_model res = get_video_contributors(58782326, limit, offset);
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

  /* get_video_stream  */
  skipped = 1;
  printf("[CI] Testing get_video_stream (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    stream_model res = get_video_stream(58782326);
    if (res.status == 1)
    {
      printf("Url: %s\n", res.url);
      printf("VideoId: %zu\n", res.videoId);
      printf("AssetPresentation: %s\n", res.assetPresentation);
      printf("VideoQuality: %s\n", res.videoQuality);
      printf("MimeType: %s\n", res.mimeType);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
}

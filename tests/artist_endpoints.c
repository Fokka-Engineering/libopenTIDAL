#include <stdlib.h>
#include <stdio.h>
#include "../src/include/openTIDAL.h"

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

  printf("Testing openTIDAL Artist Endpoints...\n");
  init_demo();

  access_token = input_token;
  
  /* get_artist  */
  skipped = 1;
  printf("[CI] Testing get_artist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    artist_model res = get_artist(4764457);
    if (res.status == 1)
    {
      size_t i;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Name: %s\n", res.name[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("Picture: %s\n", res.picture[i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
  
  /* get_artist_link  */
  skipped = 1;
  printf("[CI] Testing get_artist_link (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("Limit: ");
    scanf("%zu", &limit);
    printf("Offset: ");
    scanf("%zu", &offset);

    artist_link_model res = get_artist_link(4764457, limit, offset);
    if (res.status == 1)
    {
      int i;
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      printf("TotalNumberOfItems: %zu\n", res.totalNumberOfItems);
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Url: %s\n", res.url[i]);
        printf("SiteName: %s\n", res.siteName[i]);
      }
      printf("Source: %s\n", res.source);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_artist_mix  */
  skipped = 1;
  printf("[CI] Testing get_artist_mix (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    mix_model res = get_artist_mix(4764457);
    if (res.status == 1)
    {
      printf("Id: %s\n", res.id);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_artist_toptracks  */
  skipped = 1;
  printf("[CI] Testing get_artist_toptracks (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("Limit: ");
    scanf("%zu", &limit);
    printf("Offset: ");
    scanf("%zu", &offset);

    items_model res = get_artist_toptracks(4764457, limit, offset);
    if (res.status == 1)
    {
      size_t i;
      size_t a;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("Duration: %zu\n", res.duration[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("TrackNumber: %zu\n", res.trackNumber[i]);
        printf("VolumeNumber: %zu\n", res.volumeNumber[i]);
        printf("Version: %s\n", res.version[i]);
        printf("Quality: %s\n", res.audioQuality[i]);
        printf("Cover: %s\n", res.cover[i]);
        printf("VideoCover: %s\n", res.videoCover[i]);
	printf("Explicit: %zu\n", res.explicitItem[i]);
	printf("AllowStreaming: %zu\n", res.allowStreaming[i]);
	printf("StreamReady: %zu\n", res.streamReady[i]);
	printf("ReplayGain: %f\n", res.replayGain[i]);
	printf("Peak: %f\n", res.peak[i]);
	printf("AlbumId: %zu\n", res.albumId[i]);
        printf("AlbumTitle: %s\n", res.albumTitle[i]);
	printf("Type: %s\n", res.type[i]);
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

  /* get_artist_videos  */
  skipped = 1;
  printf("[CI] Testing get_artist_videos (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("Limit: ");
    scanf("%zu", &limit);
    printf("Offset: ");
    scanf("%zu", &offset);

    items_model res = get_artist_videos(4764457, limit, offset);
    if (res.status == 1)
    {
      size_t i;
      size_t a;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("Duration: %zu\n", res.duration[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("TrackNumber: %zu\n", res.trackNumber[i]);
        printf("VolumeNumber: %zu\n", res.volumeNumber[i]);
        printf("Quality: %s\n", res.quality[i]);
        printf("ImageId: %s\n", res.imageId[i]);
	printf("Explicit: %zu\n", res.explicitItem[i]);
	printf("AllowStreaming: %zu\n", res.allowStreaming[i]);
	printf("StreamReady: %zu\n", res.streamReady[i]);
	printf("Type: %s\n", res.type[i]);
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

  /* get_artist_albums  */
  skipped = 1;
  printf("[CI] Testing get_artist_albums (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("Limit: ");
    scanf("%zu", &limit);
    printf("Offset: ");
    scanf("%zu", &offset);

    album_model res = get_artist_albums(4764457, limit, offset);
    if (res.status == 1)
    {
      int i;
      int a;
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      printf("TotalNumberOfItems: %zu\n", res.totalNumberOfItems);
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("Duration: %zu\n", res.duration[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("NumberOfTracks: %zu\n", res.numberOfTracks[i]);
        printf("NumberOfVideos: %zu\n", res.numberOfVideos[i]);
	printf("NumberOfVolumes: %zu\n", res.numberOfVolumes[i]);
        printf("ReleaseDate: %s\n", res.releaseDate[i]);
	printf("Copyright: %s\n", res.copyright[i]);
	printf("Quality: %s\n", res.quality[i]);
        printf("Cover: %s\n", res.cover[i]);
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

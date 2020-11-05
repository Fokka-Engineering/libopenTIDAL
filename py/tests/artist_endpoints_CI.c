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

  printf("Testing openTIDAL Artist Endpoints...\n");
  printf("[CI] Enter your access_token: ");
  scanf("%s", input_token); /* Pls no BufferOverflow :( */
  access_token = input_token;
  
  /* get_artist  */
  skipped = 1;
  printf("[CI] Testing get_artist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    artist_model res = get_artist(9606);
    if (res.status == 1)
    {
      printf("Id: %zu\n", res.id[0]);
      printf("Name: %s\n", res.name[0]);
      printf("Popularity: %zu\n", res.popularity[0]);
      if (res.hasPicture[0])
      {
        printf("Picture: %s\n", res.picture[0]);
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

    artist_link_model res = get_artist_link(9606, limit, offset);
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
    mix_model res = get_artist_mix(9606);
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

    items_model res = get_artist_toptracks(9606, limit, offset);
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
        if (res.hasReleaseDate[i] == 1)
        {
	  printf("ReleaseDate: %s\n", res.releaseDate[i]);
	}
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

    items_model res = get_artist_videos(9606, limit, offset);
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
        if (res.hasReleaseDate[i] == 1)
        {
	  printf("ReleaseDate: %s\n", res.releaseDate[i]);
	}
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

    album_model res = get_artist_albums(9606, limit, offset);
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

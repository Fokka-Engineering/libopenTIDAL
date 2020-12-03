/*
  Copyright (c) 2020 Hugo Melder and openTIDAL contributors
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <openTIDAL/openTIDAL.h>

int main(void)
{
  size_t skipped;

  printf("Testing openTIDAL Album Endpoints...\n");
  //init("location/of/your/configfile");
  init_demo();
  /* get_album  */
  skipped = 1;
  printf("[CI] Testing get_album (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    album_model res = get_album(4123207);
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

  /* get_favorite_albums  */
  skipped = 1;
  printf("[CI] Testing get_favorite_albums (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit = 10;
    size_t offset = 0;
    char *order = "DATE";
    char *orderDirection = "DESC";
    album_model res = get_favorite_album(limit, offset, order, orderDirection);
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
  
  /* add_favorite_album  */
  skipped = 1;
  printf("[CI] Testing add_favorite_album (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t albumId = 25934525;
    int res = add_favorite_album(albumId);
    if (res == 1)
    {
      printf("Success!\n");
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
 
  /* delete_favorite_album  */
  skipped = 1;
  printf("[CI] Testing delete_favorite_album (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t albumId = 25934525;
    int res = delete_favorite_album(albumId);
    if (res == 1)
    {
      printf("Success!\n");
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  cleanup();
}

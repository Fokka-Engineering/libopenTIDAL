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
#include "../src/include/openTIDAL.h"

int main(void)
{
  size_t skipped;
  init("/Users/admin/Documents/Git/openTIDAL/bin/persistent.json"); 
  printf("Testing openTIDAL Playlist Endpoints...\n");

  /* get_playlist  */
  skipped = 1;
  printf("[CI] Testing get_playlist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    playlist_model res = get_playlist("854e65fb-857a-4db3-9108-4be406e8ea3d");
    if (res.status == 1)
    {
      printf("NumberOfTracks: %zu\n", res.numberOfTracks[0]);
      printf("NumberOfVideos: %zu\n", res.numberOfVideos[0]);
      printf("Uuid: %s\n", res.uuid[0]);
      printf("Title: %s\n", res.title[0]);
      printf("Description: %s\n", res.description[0]);
      printf("Duration: %zu\n", res.duration[0]);
      printf("Last Updated: %s\n", res.lastUpdated[0]);
      printf("Created: %s\n", res.created[0]);
      printf("Image: %s\n", res.image[0]);
      printf("Square Image: %s\n", res.squareImage[0]);
      printf("Popularity: %zu\n", res.popularity[0]);
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_playlist_items */
  skipped = 1;
  printf("[CI] Testing get_playlist_items (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    items_model res = get_playlist_items("854e65fb-857a-4db3-9108-4be406e8ea3d", limit, offset);
    if (res.status == 1)
    {
      int i;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("Duration: %zu\n", res.duration[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
        printf("TrackNumber: %zu\n", res.trackNumber[i]);
        printf("VolumeNumber: %zu\n", res.volumeNumber[i]);
        printf("AlbumId: %zu\n", res.albumId[i]);
        printf("AlbumTitle: %s\n", res.albumTitle[i]);
        printf("Cover: %s\n", res.cover[i]);
        printf("Quality: %s\n", res.quality[i]);
        printf("Version: %s\n", res.version[i]);
        size_t a;
        for (a = 0; a < res.subArraySize[i]; ++a)
        {
          printf("ArtistName #%zu: %s\n", a, res.artistName[i][a]);
          printf("ArtistId #%zu: %zu\n", a, res.artistId[i][a]);
        }
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* delete_playlist */
  skipped = 1;
  printf("[CI] Testing delete_playlist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    char playlist[40];
    printf("[CI] Playlist: ");
    scanf("%s", playlist);

    int res = delete_playlist(playlist);
    if (res == 1)
    {
      printf("[CI] Success!\n");
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
  
  /* delete_playlist_item */
  skipped = 1;
  printf("[CI] Testing delete_playlist_item (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    char playlist[40];
    printf("[CI] Playlist: ");
    scanf("%s", playlist);
    
    char *eTag = get_playlist_etag(playlist);
    if (eTag != 0)
    {
      int res = delete_playlist_item(playlist, 0, eTag);
      free(eTag);
      if (res == 1)
      {
        printf("[CI] Success!\n");
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* move_playlist_item */
  printf("[CI] Skipping move_playlist_item (Broken)...\n");

  /* add_playlist_item */
  printf("[CI] Skipping add_playlist_item (Broken)...\n");
  printf("[CI] Skipping add_playlist_items (Broken)...\n");
}

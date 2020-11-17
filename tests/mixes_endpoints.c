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

  printf("Testing openTIDAL Mixes Endpoints...\n");
  init_demo();
    /* get_artist_toptracks  */
  skipped = 1;
  printf("[CI] Testing get_mix_items (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    items_model res = get_mix_items("000f5db6c7c89d308459b4e6875045");
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
}

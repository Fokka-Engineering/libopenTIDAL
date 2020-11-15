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

  printf("Testing openTIDAL Video Endpoints...\n");
  
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

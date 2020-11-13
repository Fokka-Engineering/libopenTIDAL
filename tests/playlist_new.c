#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

int main()
{
  init("/Users/admin/Documents/Git/openTIDAL/bin/persistent.json");
  /*
  playlist_model res = get_playlist("854e65fb-857a-4db3-9108-4be406e8ea3d");

  printf("Title: %s\n", res.title[0]);
  printf("SquareImage: %s\n", res.squareImage[0]);

  items_model items_res = get_playlist_items("854e65fb-857a-4db3-9108-4be406e8ea3d", 10, 0);
  printf("ArraySize: %zu\n", items_res.arraySize); 
  size_t i = 0;
  for (i = 0; i < items_res.arraySize; ++i)
  {
    printf("Id: %zu\n", items_res.id[i]);
    printf("Title: %s\n", items_res.title[i]);
    printf("Duration: %zu\n", items_res.duration[i]);
    printf("Popularity: %zu\n", items_res.popularity[i]);
    printf("Version: %s\n", items_res.version[i]);
    printf("Explicit: %zu\n", items_res.explicitItem[i]);
    printf("ReplayGain: %f\n", items_res.replayGain[i]);
    printf("Cover: %s\n", items_res.cover[i]);
    printf("SubArraySize: %zu\n", items_res.subArraySize[i]);
  }
  */
  album_model res = get_album(4753719);
  if (res.status == 1)
  { 
    printf("Title: %s\n", res.title[0]);
    printf("Id: %zu\n", res.id[0]);
  }

  items_model ress = get_album_items(4753719, 10, 0);
  
  size_t a = 0;
  size_t i = 0;
  printf("TotalNumberOfItems: %zu\n", ress.totalNumberOfItems);
  printf("Limit: %zu\n", ress.limit);
  printf("Offset: %zu\n", ress.offset);
  for (i = 0; i < ress.arraySize; ++i)
  {
    printf("Title: %s\n", ress.title[i]);
    printf("Id: %zu\n", ress.id[i]);

    for (a = 0; a < ress.subArraySize[i]; ++a)
    {
      printf("Artist Name #%zu: %s\n", a, ress.artistName[i][a]);
      printf("Artist Id #%zu: %zu\n", a, ress.artistId[i][a]);
    }
  }
 
  stream_model resss = get_track_stream(4753729);
  printf("TrackId: %zu\n", resss.trackId);
  printf("Codec: %s\n", resss.codec);
  printf("Url: %s\n", resss.url);

  stream_model ressss = get_video_stream(53654641);
  printf("VideoId: %zu\n", ressss.videoId);
  printf("Url: %s\n", ressss.url);

  return 0;
}

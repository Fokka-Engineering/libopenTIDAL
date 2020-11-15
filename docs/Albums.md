###### Get Album
```c
size_t albumId = 4123207;

album_model res = get_album(albumId);
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
```

###### Get Favorite Albums
```c
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
```

###### Get Album Items
```c
size_t albumId = 4123207;
size_t limit = 10;
size_t offset = 0;

items_model res = get_album_items(albumId, limit, offset);
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
```

###### Add Favorite Album
```c
size_t albumId = 25934525;

int res = add_favorite_album(albumId);
if (res == 1)
{
  printf("Success!\n");
}
```

###### Delete Favorite Album
```c
size_t albumId = 25934525;

int res = delete_favorite_album(albumId);
if (res == 1)
{
  printf("Success!\n");
}
```
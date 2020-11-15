###### Get Artist
```c
size_t artistId = 4764457;

artist_model res = get_artist(artistId);
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
```

###### Get Favorite Artists
```c
size_t limit = 10;
size_t offset = 0;
char *order = "DATE";
char *orderDirection = "DESC";

album_model res = get_favorite_artist(limit, offset, order, orderDirection);
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
```

###### Get Artist Links
```c
size_t artistId = 4764457;
size_t limit = 10;
size_t offset = 0;

artist_link_model res = get_artist_link(artistId, limit, offset);
if (res.status == 1)
{
  size_t i;
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
```

###### Get Artist Mix
```c
size_t artistId = 4764457;

mix_model res = get_artist_mix(artistId);
if (res.status == 1)
{
  printf("Id: %s\n", res.id);
}
```

###### Get Artist Top Tracks
```c
size_t artistId = 4764457;
size_t limit = 10;
size_t offset = 0;

items_model res = get_artist_toptracks(artistId, limit, offset);
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
    
    for (a = 0; a < res.subArraySize[i]; ++a)
    {
      printf("ArtistId: %zu\n", res.artistId[i][a]);
      printf("ArtistName %s\n", res.artistName[i][a]);
    }
  }
}
```

###### Get Artist Videos
```c
size_t artistId = 4764457;
size_t limit = 10;
size_t offset = 0;

items_model res = get_artist_videos(artistId, limit, offset);
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
    printf("Cover: %s\n", res.cover[i]);
    printf("VideoCover: %s\n", res.videoCover[i]);
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
```

###### Get Artist Albums
```c
size_t artistId = 4764457;
size_t limit = 10;
size_t offset = 0;

album_model res = get_artist_albums(artistId, limit, offset);
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

###### Add Favorite Artist
```c
size_t artistId = 4764457;

int res = add_favorite_artist(artistId);
if (res == 1)
{
  printf("Success!\n");
}
```

###### Delete Favorite Artist
```c
size_t albumId = 4764457;

int res = delete_favorite_artist(artistId);
if (res == 1)
{
  printf("Success!\n");
}
```
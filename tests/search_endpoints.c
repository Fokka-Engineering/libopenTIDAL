#include <stdlib.h>
#include <stdio.h>
#include "../src/include/openTIDAL.h"

char *client_id = "8SEZWa4J1NVC5U5Y";
char *client_secret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";

int main(void)
{
  size_t skipped;
  init("/Users/admin/Documents/Git/openTIDAL/bin/persistent.json");
  printf("Testing openTIDAL Search Endpoints...\n");
  
  /* get_search  */
  skipped = 1;
  printf("[CI] Testing get_search (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    char query[512];
    size_t limit;

    printf("Search: ");
    scanf("%s", query);
    printf("Limit: ");
    scanf("%zu", &limit);

    search_model res = get_search(query, limit);
    if (res.status == 1)
    {
      int i;
      /* Artists  */ 
      /*printf("Limit: %zu\n", res.artists.limit);
      printf("Offset: %zu\n", res.artists.offset);
      printf("TotalNumberOfItems: %zu\n", res.artists.totalNumberOfItems);
      for (i = 0; i < res.artists.arraySize; ++i)
      {
        printf("Artist #%d...\n", i);
        printf("Id: %zu\n", res.artists.id[i]);
	printf("Name: %s\n", res.artists.name[i]);
	printf("Popularity: %zu\n", res.artists.popularity[i]);
	if (res.artists.hasPicture[i])
        {
	  printf("Picture: %s\n", res.artists.picture[i]);
	}
      }*/
      /* Albums */
      /*printf("Limit: %zu\n", res.albums.limit);
      printf("Offset: %zu\n", res.albums.offset);
      printf("TotalNumberOfItems: %zu\n", res.albums.totalNumberOfItems);
      for (i = 0; i < res.albums.arraySize; ++i)
      {
        int a;
	printf("Album #%d...\n", i);
	printf("Id: %zu\n", res.albums.id[i]);
	printf("Title: %s\n", res.albums.title[i]);
	printf("Duration: %zu\n", res.albums.duration[i]);
	printf("NumberOfTracks: %zu\n", res.albums.numberOfTracks[i]);
	printf("NumberOfVideos: %zu\n", res.albums.numberOfVideos[i]);
	printf("NumberOfVolumes: %zu\n", res.albums.numberOfVolumes[i]);
	printf("Cover: %s\n", res.albums.cover[i]);
	printf("Popularity: %zu\n", res.albums.popularity[i]);
	printf("Quality: %s\n", res.albums.quality[i]);
	printf("ReleaseDate: %s\n", res.albums.releaseDate[i]);
	for (a = 0; a < res.albums.subArraySize[i]; ++a)
        {
          printf("ArtistId #%d: %zu\n", a, res.albums.artistId[i][a]);
	  printf("ArtistName #%d: %s\n", a, res.albums.artistName[i][a]);
	}
      }*/
      /* Playlists */
      printf("Limit: %zu\n", res.playlists.limit);
      printf("Offset: %zu\n", res.playlists.offset);
      printf("TotalNumberOfItems: %zu\n", res.playlists.totalNumberOfItems);
      for (i = 0; i < res.playlists.arraySize; ++i)
      {
        printf("Playlist #%d...\n", i);
        printf("Uuid: %s\n", res.playlists.uuid[i]);
	printf("Title: %s\n", res.playlists.title[i]);
	printf("Duration: %zu\n", res.playlists.duration[i]);
	printf("Image: %s\n", res.playlists.image[i]);
	printf("SquareImage: %s\n", res.playlists.squareImage[i]);
	printf("Created: %s\n", res.playlists.created[i]);
	printf("LastUpdated: %s\n", res.playlists.lastUpdated[i]);
	printf("NumberOfTracks: %zu\n", res.playlists.numberOfTracks[i]);
	printf("NumberOfVideos: %zu\n", res.playlists.numberOfVideos[i]);
	printf("Popularity: %zu\n", res.playlists.popularity[i]);
      }
      /* Tracks */
      /*printf("Limit: %zu\n", res.tracks.limit);
      printf("Offset: %zu\n", res.tracks.offset);
      printf("TotalNumberOfItems: %zu\n", res.tracks.totalNumberOfItems);
      for (i = 0; i < res.tracks.arraySize; ++i)
      {
        int a;
        printf("Track #%d...\n", i);
        printf("Id: %zu\n", res.tracks.id[i]);
	printf("Title: %s\n", res.tracks.title[i]);
	printf("Duration: %zu\n", res.tracks.duration[i]);
	printf("Popularity: %zu\n", res.tracks.popularity[i]);
	printf("ReleaseDate: %s\n", res.tracks.releaseDate[i]);
	printf("Cover: %s\n", res.tracks.cover[i]);
	printf("AlbumId: %zu\n", res.tracks.albumId[i]);
	printf("AlbumTitle: %s\n", res.tracks.albumTitle[i]);
        if (res.tracks.hasVersion[i])
        {
	  printf("Version: %s\n", res.tracks.version[i]);
	}
	for (a = 0; a < res.tracks.subArraySize[i]; ++a)
        {
          printf("ArtistId: %zu\n", res.tracks.artistId[i][a]);
	  printf("ArtistName: %s\n", res.tracks.artistName[i][a]);
	}
      }*/
      /* Videos */
      /*printf("Limit: %zu\n", res.videos.limit);
      printf("Offset: %zu\n", res.videos.offset);
      printf("TotalNumberOfItems: %zu\n", res.videos.totalNumberOfItems);
      for (i = 0; i < res.videos.arraySize; ++i)
      {
        int a;
        printf("Video #%d...\n", i);
        printf("Id: %zu\n", res.videos.id[i]);
        printf("Title: %s\n", res.videos.title[i]);
        printf("Duration: %zu\n", res.videos.duration[i]);
        printf("Popularity: %zu\n", res.videos.popularity[i]);
        printf("ReleaseDate: %s\n", res.videos.releaseDate[i]);
        printf("Cover: %s\n", res.videos.cover[i]);
        for (a = 0; a < res.videos.subArraySize[i]; ++a)
        {
          printf("ArtistId: %zu\n", res.videos.artistId[i][a]);
          printf("ArtistName: %s\n", res.videos.artistName[i][a]);
        }
      }*/
      /* TopHit */
      /*if (res.isTopArtist)
      {
        printf("TopHit Artist...\n");
        printf("Id: %zu\n", res.topArtist.id[0]);
        printf("Name: %s\n", res.topArtist.name[0]);
        printf("Popularity: %zu\n", res.topArtist.popularity[0]);
        if (res.topArtist.hasPicture[0])
        {
          printf("Picture: %s\n", res.topArtist.picture[0]);
        }
      }
      if (res.isTopAlbum)
      {
        int a;
        printf("TopHit Album...\n");
        printf("Id: %zu\n", res.topAlbum.id[0]);
        printf("Title: %s\n", res.topAlbum.title[0]);
        printf("Duration: %zu\n", res.topAlbum.duration[0]);
        printf("NumberOfTracks: %zu\n", res.topAlbum.numberOfTracks[0]);
        printf("NumberOfVideos: %zu\n", res.topAlbum.numberOfVideos[0]);
        printf("NumberOfVolumes: %zu\n", res.topAlbum.numberOfVolumes[0]);
        printf("Cover: %s\n", res.topAlbum.cover[0]);
        printf("Popularity: %zu\n", res.topAlbum.popularity[0]);
        printf("Quality: %s\n", res.topAlbum.quality[0]);
        printf("ReleaseDate: %s\n", res.topAlbum.releaseDate[0]);
        for (a = 0; a < res.topAlbum.subArraySize[0]; ++a)
        {
          printf("ArtistId #%d: %zu\n", a, res.topAlbum.artistId[0][a]);
          printf("ArtistName #%d: %s\n", a, res.topAlbum.artistName[0][a]);
        }
      }
      if (res.isTopPlaylist)
      {
        printf("TopHit Playlist...\n");
        printf("Uuid: %s\n", res.topPlaylist.uuid[0]);
        printf("Title: %s\n", res.topPlaylist.title[0]);
        printf("Duration: %zu\n", res.topPlaylist.duration[0]);
        printf("Image: %s\n", res.topPlaylist.image[0]);
        printf("SquareImage: %s\n", res.topPlaylist.squareImage[0]);
        printf("Created: %s\n", res.topPlaylist.created[0]);
        printf("LastUpdated: %s\n", res.topPlaylist.lastUpdated[0]);
        printf("NumberOfTracks: %zu\n", res.topPlaylist.numberOfTracks[0]);
        printf("NumberOfVideos: %zu\n", res.topPlaylist.numberOfVideos[0]);
        printf("Popularity: %zu\n", res.topPlaylist.popularity[0]);
        if (res.topPlaylist.hasDescription[0])
        {
          printf("Description: %s\n", res.topPlaylist.description[0]);
        }
      }
      if (res.isTopTrack)
      {
        int a;
	printf("TopHit Track...\n");
        printf("Id: %zu\n", res.topTrack.id[0]);
        printf("Title: %s\n", res.topTrack.title[0]);
        printf("Duration: %zu\n", res.topTrack.duration[0]);
        printf("Popularity: %zu\n", res.topTrack.popularity[0]);
        printf("ReleaseDate: %s\n", res.topTrack.releaseDate[0]);
        printf("Cover: %s\n", res.topTrack.cover[0]);
        printf("AlbumId: %zu\n", res.topTrack.albumId[0]);
        printf("AlbumTitle: %s\n", res.topTrack.albumTitle[0]);
        if (res.topTrack.hasVersion[0])
        {
          printf("Version: %s\n", res.topTrack.version[0]);
        }
        for (a = 0; a < res.topTrack.subArraySize[0]; ++a)
        {
          printf("ArtistId: %zu\n", res.topTrack.artistId[0][a]);
          printf("ArtistName: %s\n", res.topTrack.artistName[0][a]);
        }
      }
      if (res.isTopVideo)
      {
        int a;
	printf("TopHit Video...\n");
        printf("Id: %zu\n", res.topVideo.id[0]);
        printf("Title: %s\n", res.topVideo.title[0]);
        printf("Duration: %zu\n", res.topVideo.duration[0]);
        printf("Popularity: %zu\n", res.topVideo.popularity[0]);
        printf("ReleaseDate: %s\n", res.topVideo.releaseDate[0]);
        printf("Cover: %s\n", res.topVideo.cover[0]);
        for (a = 0; a < res.topVideo.subArraySize[0]; ++a)
        {
          printf("ArtistId: %zu\n", res.topVideo.artistId[0][a]);
          printf("ArtistName: %s\n", res.topVideo.artistName[0][a]);
        }
      }*/
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
}

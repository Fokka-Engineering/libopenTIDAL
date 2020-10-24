#include <stdlib.h>
#include <stdio.h>
#include "../include/openTIDAL.h"
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>
#include "../include/base64.h"

int main(void)
{
  size_t skipped;
  init("persistent.json", "LOSSLESS", "HIGH");
  printf("Testing openTIDAL User Endpoints...\n");
  /* get_user  */
  skipped = 1;
  printf("[CI] Testing get_user (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    user_model res = get_user();
    if (res.status == 1)
    {
      printf("Username: %s\n", res.username);
      if (res.hasFirstName)
      {
        printf("First Name: %s\n", res.firstName);
      }
      if (res.hasLastName)
      {
        printf("Last Name: %s\n", res.lastName);
      }
      printf("E-Mail: %s\n", res.email);
      printf("CountryCode: %s\n", res.countryCode);
      printf("Created: %s\n", res.created);
      if (res.hasPicture)
      {
        printf("Picture: %s\n", res.picture);
      }
      if (res.hasGender)
      {
        printf("Gender: %s\n", res.gender);
      }
      if (res.hasDateOfBirth)
      {
        printf("Date of Birth: %s\n", res.dateOfBirth);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }
  /* get_user_album  */
  skipped = 1;
  printf("[CI] Testing get_user_album (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);
    
    char *order = "DATE";
    char *orderDirection = "DESC";
    album_model res = get_user_album(limit, offset, order, orderDirection);
    if (res.status == 1)
    {
      size_t i;
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      printf("TotalNumberOfItems: %zu\n", res.totalNumberOfItems);
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("[CI] Album #%zu...\n", i);
	printf("Id: %zu\n", res.id[i]);
	printf("Title: %s\n", res.title[i]);
	printf("Duration: %zu\n", res.duration[i]);
	printf("NumberOfTracks: %zu\n", res.numberOfTracks[i]);
	printf("NumberOfVideos: %zu\n", res.numberOfVideos[i]);
	printf("NumberOfVolumes: %zu\n", res.numberOfVolumes[i]);
	printf("ReleaseDate: %s\n", res.releaseDate[i]);
	printf("Copyright: %s\n", res.copyright[i]);
	printf("Cover: %s\n", res.cover[i]);
	printf("Popularity: %zu\n", res.popularity[i]);
        
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

  /* get_user_artist  */
  skipped = 1;
  printf("[CI] Testing get_user_artist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t offset;
    size_t limit;
    
    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    char *order = "DATE";
    char *orderDirection = "DESC";
    
    artist_model res = get_user_artist(limit, offset, order, orderDirection);
    if (res.status == 1)
    {
      int i;
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %zu\n", res.id[i]);
	printf("Name: %s\n", res.name[i]);
	if (res.hasPicture[i])
        {
 	  printf("Picture: %s\n", res.picture[i]);
	}
	printf("Popularity: %zu\n", res.popularity[i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* get_user_tracks  */
  skipped = 1;
  printf("[CI] Testing get_user_tracks (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    char *order = "DATE";
    char *orderDirection = "DESC";

    items_model res = get_user_tracks(limit, offset, order, orderDirection);
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
	if (res.hasVersion[i])
        {
          printf("Version: %s\n", res.version[i]);
        }
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
 
  /* get_user_videos  */
  skipped = 1;
  printf("[CI] Testing get_user_videos (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    char *order = "DATE";
    char *orderDirection = "DESC";

    items_model res = get_user_videos(limit, offset, order, orderDirection);
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
        printf("Quality: %s\n", res.quality[i]);
	if (res.hasVersion[i])
        {
          printf("Version: %s\n", res.version[i]);
        }
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

  /* get_user_playlist  */
  skipped = 1;
  printf("[CI] Testing get_user_playlist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    size_t limit;
    size_t offset;

    printf("[CI] Limit: ");
    scanf("%zu", &limit);
    printf("[CI] Offset: ");
    scanf("%zu", &offset);

    char *order = "DATE";
    char *orderDirection = "DESC";

    playlist_model res = get_user_playlist(limit, offset, order, orderDirection);
    if (res.status == 1)
    {
      int i;
      printf("Limit: %zu\n", res.limit);
      printf("Offset: %zu\n", res.offset);
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("[CI] Playlist #%d...\n", i);
        printf("TotalNumberOfItems: %zu\n", res.totalNumberOfItems);
        printf("NumberOfTracks: %zu\n", res.numberOfTracks[i]);
        printf("NumberOfVideos: %zu\n", res.numberOfVideos[i]);
        printf("Uuid: %s\n", res.uuid[i]);
        printf("Title: %s\n", res.title[i]);
        if (res.hasDescription[i] != 0)
        {
          printf("Description: %s\n", res.description[i]);
        }
        printf("Duration: %zu\n", res.duration[i]);
        printf("Last Updated: %s\n", res.lastUpdated[i]);
        printf("Created: %s\n", res.created[i]);
        printf("Image: %s\n", res.image[i]);
        printf("Square Image: %s\n", res.squareImage[i]);
        printf("Popularity: %zu\n", res.popularity[i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  } 
  
  /* get_user_mixes  */
  skipped = 1;
  printf("[CI] Testing get_user_mixes (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    page_mix_model res = get_user_mixes();
    if (res.status == 1)
    {
      int i;
      for (i = 0; i < res.arraySize; ++i)
      {
        printf("Id: %s\n", res.id[i]);
        printf("Title: %s\n", res.title[i]);
        printf("SubTitle: %s\n", res.subTitle[i]);
        printf("SmallImageUrl: %s\n", res.smallImageUrl[i]);
	printf("MediumImageUrl: %s\n", res.mediumImageUrl[i]);
	printf("LargeImageUrl: %s\n", res.largeImageUrl[i]);
      }
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  /* create_user_playlist & delete_user_playlist  */
  skipped = 1;
  printf("[CI] Testing create_user_playlist & delete_user_playlist (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    char title[80];
    char description[500];

    printf("[CI] Title: ");
    scanf("%s", title);
    printf("[CI] Description: ");
    scanf("%s", description);


    playlist_model res = create_user_playlist(title, description);
    if (res.status == 1)
    {
      printf("NumberOfTracks: %zu\n", res.numberOfTracks[0]);
      printf("NumberOfVideos: %zu\n", res.numberOfVideos[0]);
      printf("Uuid: %s\n", res.uuid[0]);
      printf("Title: %s\n", res.title[0]);
      if (res.hasDescription[0] != 0)
      {
        printf("Description: %s\n", res.description[0]);
      }
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
  
  /* favorites management  */
  skipped = 1;
  printf("[CI] Testing Favorites Management (0 to skip)...: ");
  scanf("%zu", &skipped);
  if (skipped != 0)
  {
    /* Add Test */
    int res_album = add_user_album(62930816);
    if (res_album == 1)
    {
      printf("[CI] Album Creation (Success)\n");
    }
    int res_artist = add_user_artist(3755341);
    if (res_artist == 1)
    {
      printf("[CI] Artist Creation (Success)\n");
    }
    int res_playlist = add_user_playlist("cc065c40-951f-4bf7-aaa8-e6724f53ff7f");
    if (res_playlist == 1)
    {
      printf("[CI] Playlist Creation (Success)\n");
    }
    int res_track = add_user_track(32599618);
    if (res_track == 1)
    {
      printf("[CI] Track Creation (Success)\n");
    }
    int res_video = add_user_video(70543020);
    if (res_video == 1)
    {
      printf("[CI] Video Creation (Success)\n");
    }
    /* Delete Test */
    printf("[CI] Deleting...\n");

    int ress_album = delete_user_album(62930816);
    if (ress_album == 1)
    {
      printf("[CI] Album Deletion (Success)\n");
    }
    int ress_artist = delete_user_artist(3755341);
    if (ress_artist == 1)
    {
      printf("[CI] Artist Deletion (Success)\n");
    }
    int ress_playlist = delete_user_playlist("cc065c40-951f-4bf7-aaa8-e6724f53ff7f");
    if (ress_playlist == 1)
    {
      printf("[CI] Playlist Deletion (Success)\n");
    }
    int ress_track = delete_user_track(32599618);
    if (ress_track == 1)
    {
      printf("[CI] Track Deletion (Success)\n");
    }
    int ress_video = delete_user_video(70543020);
    if (ress_video == 1)
    {
      printf("[CI] Video Deletion (Success)\n");
    }
  }
  else
  {
    printf("[CI] Skipping...\n");
  }

  cleanup();
}

#pragma once

typedef struct login_code_models
{
  char deviceCode[37]; /* deviceCode is always 36 Chars long */
  char userCode[6]; /* userCode is always 5 Chars long */
} login_code_model;

typedef struct login_token_models
{
  int status;
  size_t expires_in;
  char access_token[1000];
  char refresh_token[1000];
  char countryCode[3];
  char username[50];
  size_t userId;
  size_t appleUid;
  size_t facebookUid;
  char error[50];
} login_token_model;

/* GET  */

typedef struct user_models
{
  int status;
  size_t id;
  char username[50];
  char firstName[25];
  char lastName[25];
  char email[50];
  char countryCode[3];
  char created[29];
  char picture[37]; /* Always 36 Chars Long */
  char gender[2];
  char dateOfBirth[11];
} user_model;

typedef struct playlist_models /* TODO: Check if ArrayLimit = 50 is enough */
{
  int status;
  int arraySize; /*ArraySize from cJSON Parser*/
  int totalNumberOfItems;
  int numberOfTracks[100];
  int numberOfVideos[100];
  char uuid[100][37]; /* Always 36 Chars Long */
  char title[100][80];
  char description[100][501]; /* 500 Char Limit */
  size_t duration[100];
  char lastUpdated[100][29];
  char created[100][29];
  char image[100][37];
  char squareImage[100][37];
} playlist_model;

typedef struct items_models
{
  int status;
  int offset;
  int totalNumberOfItems;
  int arraySize;
  int subArraySize[100]; /* 2nd ArraySize of Artist Array */
  size_t id[100];
  char title[100][80];
  int explicit[100];
  size_t duration[100];
  int allowStreaming[100];
  int streamReady[100];
  int popularity[100];
  size_t artistId[100][6]; /* 2D-Array for (featured) Artists  */
  char artistName[100][6][50];
  size_t albumId[100];
  char albumTitle[100][80];
  char cover[100][37];
  char videoCover[100][37]; /* NULL except type = Music Video */
  char releaseDate[100][11]; /* Only present in playlist_item response */
  char type[100][12];
} items_model;

typedef struct search_models
{
  int status;
  int offsetArtist;
  int offsetAlbum;
  int offsetPlaylist;
  int offsetTracks;
  int offsetVideos;
  int arraySizeArtist;
  int arraySizeAlbum;
  int arraySizePlaylist;
  int arraySizeTracks;
  int arraySizeVideos;
  int totalNumberOfItemsArtist;
  int totalNumberOfItemsAlbum;
  int totalNumberOfItemsPlaylist;
  int totalNumberOfItemsTracks;
  int totalNumberOfItemsVideos;
  size_t artistId[101];
  char artistName[101][50];
  char artistPicture[101][37];
  int artistPopularity[101];
  size_t albumId[101];
  char albumTitle[101][80];
  char albumCover[101][37];
  int albumPopularity[101];
  size_t albumDuration[101];
  size_t albumArtistId[101];
  char albumArtistName[101][50];
  char playlistUUID[101][37];
  char playlistTitle[101][80];
  char playlistImage[101][37];
  char playlistSquareImage[101][37];
  int playlistPopularity[101];
  int playlistNumberOfTracks[101];
  int playlistNumberOfVideos[101];
  int playlistDuration[101];
  size_t tracksId[101];
  char tracksTitle[101][80];
  char tracksCover[101][37];
  size_t tracksArtistId[101];
  char tracksArtistName[101][50];
  size_t tracksAlbumId[101];
  char tracksAlbumTitle[101][80];
  char tracksReleaseDate[101][11];
  size_t tracksDuration[101];
  size_t videoId[101];
  char videoTitle[101][80];
  char videoImageId[101][37];
  char videoReleaseDate[101][11];
  size_t videoDuration[101];
  size_t videoArtistId[101];
  char videoArtistName[101][50];
  char topHitType[8];
} search_model;

typedef struct albums_models
{
  int status;
  int arraySize;
  int subArraySize[100]; /* 2nd ArraySize of Artist Array */
  int explicit[100];
  int duration[100];
  int allowStreaming[100];
  int streamReady[100];
  char copyright[100][50];
  int numberOfTracks[100];
  int numberOfVideos[100];
  int numberOfVolumes[100];
  int popularity[100];
  size_t artistId[100][6]; /* 2D-Array for (featured) Artists  */
  char artistName[100][6][50];
  size_t albumId[100];
  char albumTitle[100][80];
  char cover[100][37];
  char videoCover[100][37];
  char releaseDate[100][11];
} album_model;

typedef struct items_credits_models
{
  int status;
  int arraySize;
  int subArraySize;
  int offset;
  char type[100][25];
  char name[100][30];
  size_t id[100];

} items_credits_model;

typedef struct album_credits_models
{
  int status;
  int arraySize;
  char type[50][30];
} album_credits_model;

typedef struct artists_models
{
  int status;
  int arraySize;
  int offset;
  int totalNumberOfItems;
  size_t id[100];
  char name[100][50];
  char picture[100][37];
  int popularity[100];
} artist_model;

typedef struct artist_links_models
{
  int status;
  int arraySize;
  int totalNumberOfItems;
  char url[20][80];
  char siteName[20][50];
  char source[25];
} artist_link_model;

typedef struct artist_mix_models
{
  int status;
  char id[32]; /* Always 31 Chars Long  */
} artist_mix_model;


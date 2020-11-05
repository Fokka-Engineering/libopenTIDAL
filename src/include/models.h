#include <time.h>
#pragma once

typedef struct curl_models
{
  int status;
  size_t responseCode;
  char *body;
  char *header;
} curl_model;

typedef struct login_code_models
{
  int status;
  char deviceCode[37]; /* deviceCode is always 36 Chars long */
  char userCode[6]; /* userCode is always 5 Chars long */
  time_t expires_in;
} login_code_model;

typedef struct login_token_models
{
  int status;
  time_t expires_in;
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
  size_t hasFirstName;
  size_t hasLastName;
  size_t hasPicture;
  size_t hasGender;
  size_t hasDateOfBirth;
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
  size_t limit;
  size_t offset;
  size_t arraySize; /*ArraySize from cJSON Parser*/
  size_t totalNumberOfItems;
  size_t numberOfTracks[100];
  size_t numberOfVideos[100];
  char uuid[100][37]; /* Always 36 Chars Long */
  char title[100][80];
  size_t hasDescription[100];
  char description[100][1024]; /* 500 Char Limit */
  size_t duration[100];
  char lastUpdated[100][29];
  char created[100][29];
  char image[100][37];
  char squareImage[100][37];
  size_t popularity[100];
} playlist_model;

typedef struct items_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t isVideo[100];
  size_t subArraySize[100]; /* 2nd ArraySize of Artist Array */
  size_t id[100];
  char title[100][80];
  int explicit[100];
  size_t duration[100];
  int allowStreaming[100];
  int streamReady[100];
  size_t popularity[100];
  size_t trackNumber[100];
  size_t volumeNumber[100];
  size_t hasVersion[100];
  char version[100][50];
  size_t artistId[100][6]; /* 2D-Array for (featured) Artists  */
  char artistName[100][6][50];
  size_t albumId[100];
  char albumTitle[100][80];
  char cover[100][37];
  char quality[100][20];
  size_t hasReleaseDate[100];
  char releaseDate[100][11]; /* Only present in playlist_item response */
  char type[100][12];
} items_model;

typedef struct albums_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t subArraySize[100]; /* 2nd ArraySize of Artist Array */
  size_t explicit[100];
  size_t duration[100];
  size_t allowStreaming[100];
  size_t streamReady[100];
  char copyright[100][50];
  size_t numberOfTracks[100];
  size_t numberOfVideos[100];
  size_t numberOfVolumes[100];
  size_t popularity[100];
  size_t artistId[100][6]; /* 2D-Array for (featured) Artists  */
  char artistName[100][6][50];
  char quality[100][20];
  size_t id[100];
  char title[100][80];
  char cover[100][37];
  char videoCover[100][37];
  char releaseDate[100][11];
} album_model;

typedef struct items_credits_models
{
  int status;
  size_t arraySize;
  int subArraySize;
  int offset;
  char type[100][25];
  char name[100][30];
  size_t id[100];

} items_credits_model;

typedef struct album_credits_models
{
  int status;
  size_t arraySize;
  char type[50][30];
} album_credits_model;

typedef struct artists_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t id[100];
  char name[100][50];
  size_t hasPicture[100];
  char picture[100][37];
  size_t popularity[100];
} artist_model;

typedef struct artist_links_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t arraySize;
  size_t totalNumberOfItems;
  char url[20][80];
  char siteName[20][50];
  char source[25];
} artist_link_model;

typedef struct mix_models
{
  int status;
  char id[32]; /* Always 31 Chars Long  */
} mix_model;

typedef struct pages_mix_models
{
  int status;
  size_t arraySize;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  char id[20][32];
  char title[20][50];
  char subTitle[20][100];
  char smallImageUrl[50][400];
  char mediumImageUrl[20][400];
  char largeImageUrl[20][400];
} page_mix_model;

typedef struct contributors_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  char name[50][50];
  char role[50][25];
} contributor_model;

typedef struct search_models
{
  int status;
  char topHitType[10];
  int isTopAlbum;
  int isTopArtist;
  int isTopTrack;
  int isTopPlaylist;
  int isTopVideo;
  album_model albums;
  artist_model artists;
  items_model tracks;
  playlist_model playlists;
  items_model videos;
  album_model topAlbum;
  artist_model topArtist;
  items_model topTrack;
  playlist_model topPlaylist;
  items_model topVideo;
} search_model;

typedef struct stream_models
{
  int status;
  char url[512];
  size_t trackId;
  size_t videoId;
  char assetPresentation[10];
  char audioQuality[20];
  char audioMode[20];
  char videoQuality[20];
  char mimeType[30];
  char codec[10];
} stream_model;

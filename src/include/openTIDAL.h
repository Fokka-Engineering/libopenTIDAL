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

#include <time.h>

#ifndef openTIDAL__h
#define openTIDAL__h

#ifdef __cplusplus
extern "C"
{
#endif

extern char *client_id;
extern char *client_secret;
extern char *access_token;
extern char *refresh_token;
extern time_t expires_in;
extern size_t userId;
extern size_t demoEnabled;
extern char *countryCode;
extern char *audioQuality;
extern char *videoQuality;
extern char *persistentFile;

typedef struct curl_models
{
  int status; /* custom status codes for error handling  */
  size_t responseCode;
  char *body;
  char *header;
} curl_model;

typedef struct login_code_models
{
  int status;
  char deviceCode[37]; /* deviceCode is always 36 Chars long */
  char userCode[6]; /* userCode is always 5 Chars long */
  char verificationUri[16];
  char verificationUriComplete[21];
  size_t timeFrame;
  time_t expires_in;
  size_t interval;
} login_code_model;

typedef struct login_token_models
{
  int status;
  time_t expires_in;
  char access_token[1000];
  char refresh_token[1000];
  char token_type[20];
  size_t timeFrame;
  char email[100];
  char countryCode[3];
  char fullName[100];
  char firstName[80];
  char lastName[80];
  char nickname[100];
  char username[100];
  char imageId[37];
  size_t created;
  size_t updated;
  size_t userId;
  size_t facebookUid;
  char appleUid[200];
  char error[50];
} login_token_model;

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
  size_t newsletter;
  size_t acceptedEULA;
  char gender[2];
  char dateOfBirth[11];
  size_t facebookUid;
  char appleUid[200];
} user_model;

typedef struct user_subscription_models
{
  int status;
  char validUntil[30];
  char accountStatus[20];
  char type[20];
  size_t offlineGracePeriod;
  char highestSoundQuality[20];
  size_t premiumAccess;
  size_t canGetTrial;
  char paymentType[20];
} user_subscription_model;

typedef struct playlist_models
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
  char description[100][1024]; /* 500 Char Limit */
  size_t duration[100];
  char lastUpdated[100][29];
  char created[100][29];
  size_t publicPlaylist[100];
  char image[100][37];
  char squareImage[100][37];
  char type[100][20];
  size_t popularity[100];
} playlist_model;

typedef struct items_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t subArraySize[100]; /* 2nd ArraySize of Artist Array */
  size_t id[100];
  char title[100][80];
  size_t explicitItem[100];
  size_t duration[100];
  size_t allowStreaming[100];
  size_t streamReady[100];
  double replayGain[100];
  double peak[100];
  size_t popularity[100];
  size_t trackNumber[100];
  size_t volumeNumber[100];
  char version[100][50];
  size_t artistId[100][15]; /* 2D-Array for (featured) Artists  */
  char artistName[100][15][50];
  size_t albumId[100];
  char albumTitle[100][80];
  char cover[100][37];
  char videoCover[100][37];
  char imageId[100][37];
  char audioQuality[100][20];
  char quality[100][20];
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
  size_t explicitItem[100];
  size_t duration[100];
  size_t allowStreaming[100];
  size_t streamReady[100];
  char copyright[100][50];
  size_t numberOfTracks[100];
  size_t numberOfVideos[100];
  size_t numberOfVolumes[100];
  size_t popularity[100];
  size_t artistId[100][15]; /* 2D-Array for (featured) Artists  */
  char artistName[100][15][50];
  char quality[100][20];
  char version[100][50];
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
  char url[100][500];
  char siteName[100][80];
  char source[25];
} artist_link_model;

typedef struct playlist_etag_models
{
  int status;
  char id[32];
} playlist_etag_model;

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
  char id[100][32];
  char title[100][50];
  char subTitle[100][100];
  size_t smallImageWidth[100];
  size_t smallImageHeight[100];
  char smallImageUrl[100][400];
  size_t mediumImageWidth[100];
  size_t mediumImageHeight[100];
  char mediumImageUrl[100][400];
  size_t largeImageWidth[100];
  size_t largeImageHeight[100];
  char largeImageUrl[100][400];
  char mixType[100][20];
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
  char manifestMimeType[30];
  char *manifest; //Pointer to temporary allocated Manifest
  char mimeType[30];
  char codec[10];
  char encryptionType[20];
} stream_model;

/* initialize openTIDAL with persistent config / specify location for config created by OAuth */
int init(char *file_location);

/* use client_id based login (No streaming privileges & user based endpoints) */
void init_demo();

/* call this function before exiting to free allocated memory */
void cleanup();


/* oAuth2 authorization endpoints 
 * (deviceFlow based login) */

/* create an deviceCode and userCode pair */
login_code_model login_create_code();

/* request access_token and refresh_token from deviceCode 
 * or poll until the user authorizes (userCode -> link.tidal.com) */
login_token_model login_create_token(char *device_code);

/* request new access_token */
login_token_model login_refresh_token(char *refresh_token);

/* logout session (access_token) */
int logout();


/* User Endpoints */


user_model get_user();
user_subscription_model get_user_subscription();


/* Playlist Endpoints */


playlist_model get_playlist(char *playlistid);
playlist_model get_favorite_playlist(size_t limit,
                              size_t offset, char *order, char *orderDirection);
playlist_model create_playlist(char *title, char *description);

items_model get_playlist_items(char *playlistid, size_t limit, size_t offset);

playlist_etag_model get_playlist_etag(char *playlistid);
int delete_playlist(char *playlistid);
int delete_playlist_item(char *playlistid, size_t index);
int move_playlist_item(char *playlistid, size_t index, size_t toIndex);
int add_playlist_item(char *playlistid, size_t trackid, char *onDupes);
int add_playlist_items(char *playlistid, char *trackids, char *onDupes);
int delete_favorite_playlist(char *playlistid);
int add_favorite_playlist(char *playlistid);



/* Album Endpoints */


album_model get_album(size_t albumid);
items_model get_album_items(size_t albumid, size_t limit, size_t offset);
album_model get_favorite_album(size_t limit,
                              size_t offset, char *order, char *orderDirection);

int add_favorite_album(size_t albumid);
int delete_favorite_album(size_t albumid);


/* Artist Endpoints */


artist_model get_artist(size_t artistid);
artist_model get_favorite_artist(size_t limit,
                              size_t offset, char *order, char *orderDirection);
artist_link_model get_artist_link(size_t artistid, size_t limit, size_t offset);
mix_model get_artist_mix(size_t artistid);
items_model get_artist_toptracks(size_t artistid, size_t limit, size_t offset);
items_model get_artist_videos(size_t artistid, size_t limit, size_t offset);
album_model get_artist_albums(size_t artistid, size_t limit, size_t offset);

int add_favorite_artist(size_t artistid);
int delete_favorite_artist(size_t artistid);


/* Track Endpoints  */


contributor_model get_track_contributors(size_t trackid, size_t limit, size_t offset);
mix_model get_track_mix(size_t trackid);
stream_model get_track_stream(size_t trackid);
items_model get_track(size_t trackid);
items_model get_favorite_tracks(size_t limit,
                              size_t offset, char *order, char *orderDirection);

int delete_favorite_track(size_t trackid);
int add_favorite_track(size_t trackid);


/* Video Endpoints  */


items_model get_video(size_t videoid);
items_model get_favorite_videos(size_t limit,
                              size_t offset, char *order, char *orderDirection);
contributor_model get_video_contributors(size_t videoid, size_t limit, size_t offset);
stream_model get_video_stream(size_t videoid);

int add_favorite_video(size_t videoid);
int delete_favorite_video(size_t videoid);


/* Mix Endpoints  */


items_model get_mix_items(char *mixid);
page_mix_model get_favorite_mixes();


/* Search Endpoints */


search_model get_search(char *term, size_t limit);

#ifdef __cplusplus
}
#endif

#endif

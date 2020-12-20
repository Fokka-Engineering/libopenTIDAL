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

#define STRUCT_INIT_CAPACITY 4
#ifndef openTIDAL__h
#define openTIDAL__h

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct openTIDAL_LoginCodeModel
{
  char *deviceCode; /* deviceCode is always 36 Chars long */
  char *userCode; /* userCode is always 5 Chars long */
  char *verificationUri;
  char *verificationUriComplete;
  size_t timeFrame;
  time_t expires_in;
  size_t interval;
} openTIDAL_LoginCodeModel;

typedef struct openTIDAL_LoginTokenModel 
{
  time_t expires_in;
  char *access_token;
  char *refresh_token;
  char *token_type;
  size_t timeFrame;
  char *email;
  char *countryCode;
  char *fullName;
  char *firstName;
  char *lastName;
  char *nickname;
  char *username;
  char *imageId;
  size_t created;
  size_t updated;
  size_t userId;
  size_t facebookUid;
  char *appleUid;
  char *error;
} openTIDAL_LoginTokenModel;

typedef struct openTIDAL_UserModel
{
  size_t id;
  char *username;
  char *firstName;
  char *lastName;
  char *email;
  char *countryCode;
  char *created;
  char *picture; /* Always 36 Chars Long */
  size_t newsletter;
  size_t acceptedEULA;
  char *gender;
  char *dateOfBirth;
  size_t facebookUid;
  char *appleUid;
} openTIDAL_UserModel;

typedef struct openTIDAL_UserSubscriptionModel
{
  char *validUntil;
  char *accountStatus;
  char *type;
  size_t offlineGracePeriod;
  char *highestSoundQuality;
  size_t premiumAccess;
  size_t canGetTrial;
  char *paymentType;
} openTIDAL_UserSubscriptionModel;

typedef struct openTIDAL_PlaylistModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t numberOfTracks;
  size_t numberOfVideos;
  char *uuid; /* Always 36 Chars Long */
  char *title;
  char *description;
  size_t duration;
  char *lastUpdated;
  char *created;
  size_t publicPlaylist;
  char *image;
  char *squareImage;
  char *type;
  size_t popularity;
} openTIDAL_PlaylistModel;

typedef struct openTIDAL_ItemsModel 
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  int subArraySize; /* 2nd ArraySize of Artist Array */
  size_t id;
  char *title;
  size_t explicitItem;
  size_t duration;
  size_t allowStreaming;
  size_t streamReady;
  double replayGain;
  double peak;
  size_t popularity;
  size_t trackNumber;
  size_t volumeNumber;
  char *version;
  size_t *artistId;
  char **artistName;
  size_t albumId;
  char *albumTitle;
  char *cover;
  char *videoCover;
  char *imageId;
  char *audioQuality;
  char *quality;
  char *type;
} openTIDAL_ItemsModel;

typedef struct openTIDAL_AlbumModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  int subArraySize; /* 2nd ArraySize of Artist Array */
  size_t explicitItem;
  size_t duration;
  size_t allowStreaming;
  size_t streamReady;
  char *copyright;
  size_t numberOfTracks;
  size_t numberOfVideos;
  size_t numberOfVolumes;
  size_t popularity;
  size_t *artistId; /* 2D-Array for (featured) Artists  */
  char **artistName;
  char *quality;
  char *version;
  size_t id;
  char *title;
  char *cover;
  char *videoCover;
  char *releaseDate;
} openTIDAL_AlbumModel;

typedef struct openTIDAL_CreditsModel
{
  size_t limit;
  size_t offset;
  char *type;
  char *name;
  size_t id;

} openTIDAL_CreditsModel;

typedef struct openTIDAL_ArtistModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t id;
  char *name;
  char *picture;
  size_t popularity;
} openTIDAL_ArtistModel;

typedef struct openTIDAL_LinkModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  char *url;
  char *siteName;
  char *source;
} openTIDAL_LinkModel;

typedef struct openTIDAL_ETagModel
{
  int status;
  char *id;
} openTIDAL_ETagModel;

typedef struct openTIDAL_MixModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  char *id;
  char *title;
  char *subTitle;
  size_t smallImageWidth;
  size_t smallImageHeight;
  char *smallImageUrl;
  size_t mediumImageWidth;
  size_t mediumImageHeight;
  char *mediumImageUrl;
  size_t largeImageWidth;
  size_t largeImageHeight;
  char *largeImageUrl;
  char *mixType;
} openTIDAL_MixModel;

typedef struct openTIDAL_ContributorModel
{
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  char *name;
  char *role;
} openTIDAL_ContributorModel;

typedef struct openTIDAL_StreamModel
{
  char *url;
  size_t trackId;
  size_t videoId;
  char *assetPresentation;
  char *audioQuality;
  char *audioMode;
  char *videoQuality;
  char *manifestMimeType;
  char *manifest; //Pointer to temporary allocated Manifest
  char *mimeType;
  char *codec;
  char *encryptionType;
} openTIDAL_StreamModel;

typedef struct openTIDAL_ConfigModel
{
  char *location; /* file location of persistent json */
  char *newStream; /* stream of newly created JSON */
  void *stream; /* pointer to allocated cJSON persistent JSON */
  void *refreshRequest; 
  void *tokenRequest;
  int demoEnabled; /* boolean to specify authorization mode */

  const char *clientId;
  const char *clientSecret;
  const char *baseUrl;
  const char *authUrl;
  
  char *username;
  size_t userId;
  time_t expiresIn;
  char *countryCode;
  
  char *accessToken;
  char *refreshToken;

  char *audioQuality;
  char *videoQuality;
} openTIDAL_ConfigModel;

/* main structure with dynamic arrays (See memory management of main struct) */
typedef struct openTIDAL
{
  openTIDAL_LoginCodeModel code;
  openTIDAL_LoginTokenModel token;
  
  openTIDAL_UserModel user;
  openTIDAL_UserSubscriptionModel subscription;

  /* pointer to dynamic arrays */
  openTIDAL_AlbumModel *albums; /* Index (Capacity & Total): 0 */
  openTIDAL_ItemsModel *items; /* Index (Capacity & Total): 1 */
  openTIDAL_ArtistModel *artists; /* Index (Capacity & Total): 2 */
  openTIDAL_PlaylistModel *playlists; /* Index (Capacity & Total): 3 */
  openTIDAL_MixModel *mixes; /* Index (Capacity & Total): 4 */

  openTIDAL_ContributorModel *contributors; /* Index (Capacity & Total): 5 */
  openTIDAL_CreditsModel *credits; /* Index (Capacity & Total): 6 */
  openTIDAL_LinkModel *links; /* Index (Capacity & Total): 7 */

  openTIDAL_StreamModel stream;

  int status; /* custom status of request */
  int capacity[8]; /* used to store capacity of each array */
  int total[8]; /* used to store amount of items in each array */
  void *json; /* pointer to allocated cJSON master tree (needed for deallocation) */
  void *jsonManifest; /* pointer to allocated cJSON master tree of stream manifest */
} openTIDAL;

/* config struct */
extern openTIDAL_ConfigModel config;

/* initialize openTIDAL with persistent config / specify location for config created by OAuth */
int openTIDAL_Init(const char *file_location);

/* enable verbose mode stdout (enables cURL verbose and openTIDAL verbose) */
void openTIDAL_Verbose();

/* call this function before exiting to free allocated memory */
void openTIDAL_Cleanup();


/* memory management of main struct (Heap Allocation & Deallocation)
 * pointer to struct parameter to avoid a copy in stack  */


/* initialize struct and it's content */
void openTIDAL_StructInit(openTIDAL *o);
/* allocate specified array in heap */
void openTIDAL_StructAlloc(openTIDAL *o, int index);
/* deallocate content in struct */
void openTIDAL_StructDelete(openTIDAL *o);

/* add model to array (Allocate the correct array before adding!) */
void openTIDAL_StructAddAlbum(openTIDAL *o, openTIDAL_AlbumModel album);
void openTIDAL_StructAddItem(openTIDAL *o, openTIDAL_ItemsModel item);
void openTIDAL_StructAddArtist(openTIDAL *o, openTIDAL_ArtistModel artist);
void openTIDAL_StructAddPlaylist(openTIDAL *o, openTIDAL_PlaylistModel playlist);
void openTIDAL_StructAddMix(openTIDAL *o, openTIDAL_MixModel mix);
void openTIDAL_StructAddContributor(openTIDAL *o, openTIDAL_ContributorModel contributor);
void openTIDAL_StructAddCredit(openTIDAL *o, openTIDAL_CreditsModel credit);
void openTIDAL_StructAddLink(openTIDAL *o, openTIDAL_LinkModel link);

/* oAuth2 authorization endpoints 
 * (deviceFlow based login) */

/* create an deviceCode and userCode pair */
openTIDAL openTIDAL_CreateLoginCode();

/* request access_token and refresh_token from deviceCode 
 * or poll until the user authorizes (userCode -> link.tidal.com) */
openTIDAL openTIDAL_CreateLoginToken(char *device_code);

/* request new access_token */
openTIDAL openTIDAL_RefreshLoginToken(char *refresh_token);

/* logout session (access_token) */
int openTIDAL_Logout();


/* User Endpoints */


openTIDAL openTIDAL_GetUser();
openTIDAL openTIDAL_GetUserSubscription();


/* Playlist Endpoints */


openTIDAL openTIDAL_GetPlaylist(const char *playlistid);
openTIDAL openTIDAL_GetFavoritePlaylists(const size_t limit,
                              const size_t offset, const char *order, const char *orderDirection);
openTIDAL openTIDAL_CreatePlaylist(const char *title, const char *description);

openTIDAL openTIDAL_GetPlaylistItems(const char *playlistid, const size_t limit, const size_t offset);

int openTIDAL_DeletePlaylist(const char *playlistid);
int openTIDAL_DeletePlaylistItem(const char *playlistid, const size_t index);
int openTIDAL_MovePlaylistItem(const char *playlistid, const size_t index, const size_t toIndex);
int openTIDAL_AddPlaylistItem(const char *playlistid, const size_t trackid, const char *onDupes);
int openTIDAL_AddPlaylistItems(const char *playlistid, const char *trackids, const char *onDupes);
int openTIDAL_DeleteFavoritePlaylist(const char *playlistid);
int openTIDAL_AddFavoritePlaylist(const char *playlistid);



/* Album Endpoints */


openTIDAL openTIDAL_GetAlbum(const size_t albumid);
openTIDAL openTIDAL_GetAlbumItems(const size_t albumid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetFavoriteAlbums(const size_t limit,
                              const size_t offset, const char *order, const char *orderDirection);

int openTIDAL_AddFavoriteAlbum(const size_t albumid);
int openTIDAL_DeleteFavoriteAlbum(const size_t albumid);


/* Artist Endpoints */


openTIDAL openTIDAL_GetArtist(const size_t artistid);
openTIDAL openTIDAL_GetFavoriteArtists(const size_t limit,
                              size_t const offset, const char *order, const char *orderDirection);
openTIDAL openTIDAL_GetArtistLink(const size_t artistid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetArtistMix(const size_t artistid);
openTIDAL openTIDAL_GetArtistTopTracks(const size_t artistid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetArtistVideos(const size_t artistid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetArtistAlbums(const size_t artistid, const size_t limit, const size_t offset);

int openTIDAL_AddFavoriteArtist(const size_t artistid);
int openTIDAL_DeleteFavoriteArtist(const size_t artistid);


/* Track Endpoints  */


openTIDAL openTIDAL_GetTrackContributors(const size_t trackid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetTrackMix(const size_t trackid);
openTIDAL openTIDAL_GetTrackStream(const size_t trackid);
openTIDAL openTIDAL_GetTrack(const size_t trackid);
openTIDAL openTIDAL_GetFavoriteTracks(const size_t limit,
                              const size_t offset, const char *order, const char *orderDirection);

int openTIDAL_DeleteFavoriteTrack(const size_t trackid);
int openTIDAL_AddFavoriteTrack(const size_t trackid);


/* Video Endpoints  */


openTIDAL openTIDAL_GetVideo(const size_t videoid);
openTIDAL openTIDAL_GetFavoriteVideos(const size_t limit,
                              const size_t offset, const char *order, const char *orderDirection);
openTIDAL openTIDAL_GetVideoContributors(const size_t videoid, const size_t limit, const size_t offset);
openTIDAL openTIDAL_GetVideoStream(const size_t videoid);

int openTIDAL_AddFavoriteVideo(const size_t videoid);
int openTIDAL_DeleteFavoriteVideo(const size_t videoid);


/* Mix Endpoints  */


openTIDAL openTIDAL_GetMixItems(const char *mixid);
openTIDAL openTIDAL_GetFavoriteMixes();


/* Search Endpoints */


openTIDAL openTIDAL_SearchAll(char *term, const size_t limit);

#ifdef __cplusplus
}
#endif

#endif

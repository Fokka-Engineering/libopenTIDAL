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

#define STRUCT_INIT_CAPACITY 10 
#ifndef openTIDAL__h
#define openTIDAL__h

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct openTIDAL_LoginCodeContainer
{
    char *deviceCode; /* deviceCode is always 36 Chars long */
    char *userCode; /* userCode is always 5 Chars long */
    char *verificationUri;
    char *verificationUriComplete;
    size_t timeFrame;
    time_t expires_in;
    int interval;
} openTIDAL_LoginCodeContainer;

typedef struct openTIDAL_LoginTokenContainer 
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
} openTIDAL_LoginTokenContainer;

typedef struct openTIDAL_UserContainer
{
    size_t id;
    char *username;
    char *firstName;
    char *lastName;
    char *email;
    char *countryCode;
    char *created;
    char *picture; /* Always 36 Chars Long */
    int newsletter;
    int acceptedEULA;
    char *gender;
    char *dateOfBirth;
    size_t facebookUid;
    char *appleUid;
} openTIDAL_UserContainer;

typedef struct openTIDAL_UserSubscriptionContainer
{
    char *validUntil;
    char *accountStatus;
    char *type;
    size_t offlineGracePeriod;
    char *highestSoundQuality;
    int premiumAccess;
    int canGetTrial;
    char *paymentType;
} openTIDAL_UserSubscriptionContainer;

typedef struct openTIDAL_PlaylistContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    int numberOfTracks;
    int numberOfVideos;
    char *uuid; /* Always 36 Chars Long */
    char *title;
    char *description;
    size_t duration;
    char *lastUpdated;
    char *created;
    int publicPlaylist;
    char *image;
    char *squareImage;
    char *type;
    int popularity;
} openTIDAL_PlaylistContainer;

typedef struct openTIDAL_ItemsContainer 
{
    int limit;
    int offset;
    int totalNumberOfItems;
    int subArraySize; /* 2nd ArraySize of Artist Array */
    size_t id;
    char *title;
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    double replayGain;
    double peak;
    int popularity;
    int trackNumber;
    int volumeNumber;
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
    int isVideo;
} openTIDAL_ItemsContainer;

typedef struct openTIDAL_AlbumContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    int subArraySize; /* 2nd ArraySize of Artist Array */
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    char *copyright;
    int numberOfTracks;
    int numberOfVideos;
    int numberOfVolumes;
    int popularity;
    size_t *artistId; /* 2D-Array for (featured) Artists    */
    char **artistName;
    char *quality;
    char *version;
    size_t id;
    char *title;
    char *cover;
    char *videoCover;
    char *releaseDate;
} openTIDAL_AlbumContainer;

typedef struct openTIDAL_CreditsContainer
{
    int limit;
    int offset;
    char *type;
    char *name;
    size_t id;

} openTIDAL_CreditsContainer;

typedef struct openTIDAL_ArtistContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    size_t id;
    char *name;
    char *picture;
    int popularity;
} openTIDAL_ArtistContainer;

typedef struct openTIDAL_LinkContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    char *url;
    char *siteName;
    char *source;
} openTIDAL_LinkContainer;

typedef struct openTIDAL_ETagContainer
{
    int status;
    char *id;
} openTIDAL_ETagContainer;

typedef struct openTIDAL_MixContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    char *id;
    char *title;
    char *subTitle;
    int smallImageWidth;
    int smallImageHeight;
    char *smallImageUrl;
    int mediumImageWidth;
    int mediumImageHeight;
    char *mediumImageUrl;
    int largeImageWidth;
    int largeImageHeight;
    char *largeImageUrl;
    char *mixType;
} openTIDAL_MixContainer;

typedef struct openTIDAL_ContributorContainer
{
    int limit;
    int offset;
    int totalNumberOfItems;
    char *name;
    char *role;
} openTIDAL_ContributorContainer;

typedef struct openTIDAL_StreamContainer
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
} openTIDAL_StreamContainer;

/* implementing personalised modules of home page api */
typedef struct openTIDAL_PageHomeContainer
{
    /* the recentlyPlayed module is a mixed type module.
     * A mixed type module can contain albums, playlists, mixes and items. 
     * 
     * The recentlyPlayedTypes array contains the information needed to 
     * reconstruct this module.
     * You can't store different types in the same structure, 
     * that's why the reconstruction is necessary.
     *
     * Example:
     * recentlyPlayed_Start = 0;
     * recentlyPlayed_ArrayTypes = {0, 1, 2, 3, 4};
     * recentlyPlayed_ArrayPosition = {0, 2, 4, 5, 1};
     * recentlyPlayed_Total = 5;
     *
     * Use the identifiers of the openTIDAL Struct (Documented below) to
     * access the correct array.
     * The first item is an Album (0 = Album) and Position 0.
     * The second item is a Video or a Track (1 = Items) and Position 2. 
     * The third item is an Artist (2 = Artist) and Position 4.
     * The fourth item is a Playlist (3 = Playlist) and Position 5.
     * The fifth item is a Mix (4 = Mix) and Position 1.
     *
     * The Array Position specifies the position in the corresponding array.
     * The Start Parameter Defines the array start position of the allocated 
     * array(s) inside the openTIDAL struct 
     */
    int *recentlyPlayed_ArrayTypes;
    int *recentlyPlayed_ArrayPosition;
    int recentlyPlayed_Total;

    int mixesForYou_Start;
    int mixesForYou_Total;
    
    int *radioStationsForYou_Start; /* a radioStation is a Mix */
    int *radioStationsForYou_Total;
    int radioStationsForYou_ArraySize;

    int yourHistory_Start; /* Mix List */
    int yourHistory_Total;

    int featuredPlaylists_Start;
    int featuredPlaylists_Total;

    int *becauseYouListenedTo_Start;
    char **becauseYouListenedTo_Title;
    int *becauseYouListenedTo_Total;
    int becauseYouListenedTo_ArraySize;

} openTIDAL_PageHomeContainer;

typedef struct openTIDAL_SessionContainer
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
} openTIDAL_SessionContainer;

/* main structure with dynamic arrays (See memory management of main struct) */
typedef struct openTIDAL_ContentContainer
{
    openTIDAL_LoginCodeContainer code;
    openTIDAL_LoginTokenContainer token;
    
    openTIDAL_UserContainer user;
    openTIDAL_UserSubscriptionContainer subscription;

    /* pointer to dynamic arrays */
    openTIDAL_AlbumContainer *albums; /* Index (Capacity & Total): 0 */
    openTIDAL_ItemsContainer *items; /* Index (Capacity & Total): 1 */
    openTIDAL_ArtistContainer *artists; /* Index (Capacity & Total): 2 */
    openTIDAL_PlaylistContainer *playlists; /* Index (Capacity & Total): 3 */
    openTIDAL_MixContainer *mixes; /* Index (Capacity & Total): 4 */

    openTIDAL_ContributorContainer *contributors; /* Index (Capacity & Total): 5 */
    openTIDAL_CreditsContainer *credits; /* Index (Capacity & Total): 6 */
    openTIDAL_LinkContainer *links; /* Index (Capacity & Total): 7 */

    openTIDAL_StreamContainer stream;
    openTIDAL_PageHomeContainer home;

    int status; /* custom status of request */
    int capacity[8]; /* used to store capacity of each array */
    int total[8]; /* used to store amount of items in each array */
    void *json; /* pointer to allocated cJSON master tree (needed for deallocation) */
    void *jsonManifest; /* pointer to allocated cJSON master tree of stream manifest */
} openTIDAL_ContentContainer;

/* initialize openTIDAL with persistent config / specify location for config created by OAuth */
const int openTIDAL_SessionInit(openTIDAL_SessionContainer *session, const char *file_location);

/* enable verbose mode stdout (enables cURL verbose and openTIDAL verbose) */
void openTIDAL_Verbose(int loglevel);
void openTIDAL_ParseVerbose(const char *prefix, const char *string, int loglevel);
int openTIDAL_GetLogLevel();
/* call this function before exiting to free allocated memory */
void openTIDAL_Cleanup();


/* memory management of main struct (Heap Allocation & Deallocation)
 * pointer to struct parameter to avoid a copy in stack  */


/* initialize struct and it's content */
void openTIDAL_StructInit(openTIDAL_ContentContainer *o);
/* allocate specified array in heap */
void openTIDAL_StructAlloc(openTIDAL_ContentContainer *o, int index);
/* deallocate content in struct */
void openTIDAL_StructDelete(openTIDAL_ContentContainer *o);

/* add model to array (Allocate the correct array before adding!) */
void openTIDAL_StructAddAlbum(openTIDAL_ContentContainer *o, openTIDAL_AlbumContainer album);
void openTIDAL_StructAddItem(openTIDAL_ContentContainer *o, openTIDAL_ItemsContainer item);
void openTIDAL_StructAddArtist(openTIDAL_ContentContainer *o, openTIDAL_ArtistContainer artist);
void openTIDAL_StructAddPlaylist(openTIDAL_ContentContainer *o, openTIDAL_PlaylistContainer playlist);
void openTIDAL_StructAddMix(openTIDAL_ContentContainer *o, openTIDAL_MixContainer mix);
void openTIDAL_StructAddContributor(openTIDAL_ContentContainer *o, openTIDAL_ContributorContainer contributor);
void openTIDAL_StructAddCredit(openTIDAL_ContentContainer *o, openTIDAL_CreditsContainer credit);
void openTIDAL_StructAddLink(openTIDAL_ContentContainer *o, openTIDAL_LinkContainer link);

/* Session */

const int
openTIDAL_SessionInit(openTIDAL_SessionContainer *session, const char *file_location);
void openTIDAL_SessionCreateFile(openTIDAL_SessionContainer *session);
void openTIDAL_SessionRefresh(openTIDAL_SessionContainer *session);
void openTIDAL_SessionCleanup(openTIDAL_SessionContainer *session);


/* oAuth2 authorization endpoints 
 * (deviceFlow based login) */


/* create an deviceCode and userCode pair */
openTIDAL_ContentContainer
openTIDAL_AuthCreateUserCode(openTIDAL_SessionContainer *session);

/* request access_token and refresh_token from deviceCode 
 * or poll until the user authorizes (userCode -> link.tidal.com) */
openTIDAL_ContentContainer
openTIDAL_AuthCreateBearerToken(openTIDAL_SessionContainer *session, char *device_code);

/* request new access_token */
openTIDAL_ContentContainer
openTIDAL_AuthRefreshBearerToken(openTIDAL_SessionContainer *session, char *refresh_token);

/* logout session (access_token) */
int openTIDAL_AuthLogout(openTIDAL_SessionContainer *session);


/* User Endpoints */


openTIDAL_ContentContainer openTIDAL_GetUser();
openTIDAL_ContentContainer openTIDAL_GetUserSubscription();


/* Playlist Endpoints */


openTIDAL_ContentContainer
openTIDAL_GetPlaylist(const char *playlistid);

openTIDAL_ContentContainer
openTIDAL_GetFavoritePlaylists(const int limit,
    const int offset, const char *order, const char *orderDirection);

openTIDAL_ContentContainer
openTIDAL_CreatePlaylist(const char *title, const char *description);

openTIDAL_ContentContainer
openTIDAL_GetPlaylistItems(const char *playlistid, const int limit, const int offset);

int openTIDAL_DeletePlaylist(const char *playlistid);
int openTIDAL_DeletePlaylistItem(const char *playlistid, const int index);
int openTIDAL_MovePlaylistItem(const char *playlistid, const int index, const int toIndex);
int openTIDAL_AddPlaylistItem(const char *playlistid, const int trackid, const char *onDupes);
int openTIDAL_AddPlaylistItems(const char *playlistid, const char *trackids, const char *onDupes);
int openTIDAL_DeleteFavoritePlaylist(const char *playlistid);
int openTIDAL_AddFavoritePlaylist(const char *playlistid);



/* Album Endpoints */


openTIDAL_ContentContainer openTIDAL_GetAlbum(openTIDAL_SessionContainer *session, 
        const size_t albumid);

openTIDAL_ContentContainer openTIDAL_GetAlbumItems(openTIDAL_SessionContainer *session,
        const size_t albumid, const int limit, const int offset);

openTIDAL_ContentContainer openTIDAL_GetFavoriteAlbums(openTIDAL_SessionContainer *session, 
        const int limit, const int offset, const char *order, const char *orderDirection);

int
openTIDAL_AddFavoriteAlbum(openTIDAL_SessionContainer *session, const size_t albumid);

int
openTIDAL_DeleteFavoriteAlbum(openTIDAL_SessionContainer *session, const size_t albumid);


/* Artist Endpoints */


openTIDAL_ContentContainer
openTIDAL_GetArtist(openTIDAL_SessionContainer *session, const size_t artistid);

openTIDAL_ContentContainer
openTIDAL_GetFavoriteArtists(openTIDAL_SessionContainer *session, const int limit,
    const int offset, const char *order, const char *orderDirection);

openTIDAL_ContentContainer
openTIDAL_GetArtistLink(openTIDAL_SessionContainer *session,
        const size_t artistid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetArtistMix(openTIDAL_SessionContainer *session, 
        const size_t artistid);

openTIDAL_ContentContainer
openTIDAL_GetArtistTopTracks(openTIDAL_SessionContainer *session,
        const size_t artistid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetArtistVideos(openTIDAL_SessionContainer *session,
        const size_t artistid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetArtistAlbums(openTIDAL_SessionContainer *session, 
        const size_t artistid, const int limit, const int offset);

int 
openTIDAL_AddFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid);

int 
openTIDAL_DeleteFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid);


/* Track Endpoints  */


openTIDAL_ContentContainer
openTIDAL_GetTrackContributors(const size_t trackid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetTrackMix(const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetTrackStream(const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetTrack(const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetFavoriteTracks(const int limit,
    const int offset, const char *order, const char *orderDirection);

int
openTIDAL_DeleteFavoriteTrack(const size_t trackid);

int
openTIDAL_AddFavoriteTrack(const size_t trackid);


/* Video Endpoints  */


openTIDAL_ContentContainer openTIDAL_GetVideo(const size_t videoid);
openTIDAL_ContentContainer openTIDAL_GetFavoriteVideos(const int limit,
    const int offset, const char *order, const char *orderDirection);
openTIDAL_ContentContainer openTIDAL_GetVideoContributors(const size_t videoid, const int limit, const int offset);
openTIDAL_ContentContainer openTIDAL_GetVideoStream(const size_t videoid);

int openTIDAL_AddFavoriteVideo(const size_t videoid);
int openTIDAL_DeleteFavoriteVideo(const size_t videoid);


/* Mix Endpoints    */


openTIDAL_ContentContainer openTIDAL_GetMixItems(const char *mixid);
openTIDAL_ContentContainer openTIDAL_GetFavoriteMixes();


/* Search Endpoints */


openTIDAL_ContentContainer openTIDAL_SearchAll(char *term, const int limit);


/* Page Endpoints */


openTIDAL_ContentContainer openTIDAL_GetHome();
#ifdef __cplusplus
}
#endif

#endif

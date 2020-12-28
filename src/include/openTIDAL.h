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

typedef struct openTIDAL_LoginCodeContainer {
    /* The Device Code is a 128bit uuid used to verify the session between
     * openTIDAL and the authorization server. OpenTIDAL uses the uuid
     * to request the access token from the authorization server.
     * OAuth2 Grant Type: urn:ietf:params:oauth:grant-type:device_code */
    char *deviceCode;
    /* The User Code is a 5 character string linked to the Device Code.
     * It's shown to the user and used to identify the session in a
     * web browser. */
    char *userCode;
    
    /* The URI the user should go to with the User Code in order to
     * sign in. */ 
    char *verificationUri;
    char *verificationUriComplete;
    
    /* The number of seconds before the Device Code and User Code expire. */ 
    size_t timeFrame;
    /* Calculated Unix timestamp */
    time_t expires_in;
    /* The number of seconds the client should wait between polling
     * requests. */
    int interval;
} openTIDAL_LoginCodeContainer;

typedef struct openTIDAL_LoginTokenContainer { 
    /* Access Token issued by Authorization Server with a limited
     * lifespan. See timeFrame and expires_in. */
    char *access_token;
    /* After the Access Token expired the Refresh Token is used
     * to renew the Access Token. It has no expiry date.  */
    char *refresh_token;
    /* Always "Bearer" */
    char *token_type;

    /* The number of seconds before the Access Token expires. */ 
    size_t timeFrame;
    /* Calculated Unix timestamp */
    time_t expires_in;
    /* Internal error handling of token request */
    char *error;
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
    /* Only present if third-party oauth authentication is being
     * used or the TIDAL account is linked to a facebook account. */
    size_t facebookUid;
    char *appleUid;
} openTIDAL_LoginTokenContainer;

typedef struct openTIDAL_UserContainer {
    size_t id;
    char *username;
    char *firstName;
    char *lastName;
    char *email;
    char *countryCode;
    char *created;
    char *picture;
    int newsletter;
    int acceptedEULA;
    char *gender;
    char *dateOfBirth;
    /* Only present if third-party oauth authentication is being
     * used or the TIDAL account is linked to a facebook account. */
    size_t facebookUid;
    char *appleUid;
} openTIDAL_UserContainer;

typedef struct openTIDAL_UserSubscriptionContainer {
    /* Subscription timeframe until next bill  */
    char *validUntil;
    char *accountStatus;
    /* TIDAL subscription type */
    char *type;
    /* The Offline Grace Period defines the time a
     * client with offline capabilities can be offline without
     * reauthenticating. This prevents the user from playing
     * offline content with an inactive subscription. OpenTIDAL
     * does not uses TIDALs offline functionalities. */
    size_t offlineGracePeriod;
    char *highestSoundQuality;
    int premiumAccess;
    int canGetTrial;
    /* Family (Parent) or direct. */
    char *paymentType;
} openTIDAL_UserSubscriptionContainer;

typedef struct openTIDAL_PlaylistContainer {
    /* Applied limit by the api. */
    int limit;
    /* Applied offset by the api. */
    int offset;
    int totalNumberOfItems;
    int numberOfTracks;
    int numberOfVideos;
    char *uuid;
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

typedef struct openTIDAL_ItemsContainer { 
    int limit;
    int offset;
    int totalNumberOfItems;
    /* Arraysize of artist arrays (artistId & artistName). */
    int subArraySize;
    size_t id;
    char *title;
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    int popularity;
    int trackNumber;
    int volumeNumber;
    size_t *artistId;
    char **artistName;

    /* Only available if item is a track.
     * (isVideo: FALSE) */
    double replayGain;
    double peak;
    char *audioQuality;
    char *version;
    size_t albumId;
    char *albumTitle;
    /* Album cover 128bit uuid.
     * 80x80, 160x160, 320x320, 640x640, 1280x1280 */
    char *cover;
    /* An animated album cover is a new TIDAL feature.
     * 80x80, 160x160, 320x320, 640x640, 1280x1280  */
    char *videoCover;
   
    /* Only available if item is a music video.
     * (isVideo: TRUE) */ 
    char *imageId;
    char *quality;
    
    int isVideo;
} openTIDAL_ItemsContainer;

typedef struct openTIDAL_AlbumContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    /* Arraysize of artist arrays (artistId & artistName). */
    int subArraySize;
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    char *copyright;
    int numberOfTracks;
    int numberOfVideos;
    int numberOfVolumes;
    int popularity;
    size_t *artistId;
    char **artistName;
    char *quality;
    char *version;
    size_t id;
    char *title;
    char *cover;
    /* An animated album cover is a new TIDAL feature. */
    char *videoCover;
    char *releaseDate;
} openTIDAL_AlbumContainer;

typedef struct openTIDAL_CreditsContainer {
    int limit;
    int offset;
    char *type;
    char *name;
    size_t id;

} openTIDAL_CreditsContainer;

typedef struct openTIDAL_ArtistContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    size_t id;
    char *name;
    char *picture;
    int popularity;
} openTIDAL_ArtistContainer;

typedef struct openTIDAL_LinkContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *url;
    char *siteName;
    char *source;
} openTIDAL_LinkContainer;

typedef struct openTIDAL_ETagContainer {
    int status;
    char *id;
} openTIDAL_ETagContainer;

typedef struct openTIDAL_MixContainer {
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

typedef struct openTIDAL_ContributorContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *name;
    char *role;
} openTIDAL_ContributorContainer;

typedef struct openTIDAL_StreamContainer {
    char *url;
    size_t trackId;
    size_t videoId;
    char *assetPresentation;
    char *audioQuality;
    char *audioMode;
    char *videoQuality;
    char *manifestMimeType;
    char *manifest;
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

typedef struct openTIDAL_SessionContainer {
    /* File location of persistent session config.
     * If the pointer is NULL restricted authorization
     * is enabled. */
    char *location;
    
    /* Stream of a newly created json session config.
     * Allocated in heap if a new persistent config is
     * created or updated.  */
    char *newStream;
    /* Void pointer to allocated session config. Typecasted
     * cJSON Structure. If a persistent session config is
     * specified the file is loaded into heap and parsed with
     * cJSON. The cJSON Structure gets deallocated with
     * openTIDAL_SessionCleanup */
    void *stream;
    /* Part of the SessionAuthAutoRenewal process. Typecasted
     * cJSON Structure. Allocated after refresh request is performed.
     * Old cJSON Structure from previous request will be deallocated
     * prior. */
    void *refreshRequest;
    /* Part of the AuthCreateBearerToken process. Typecasted cJSON
     * Structure. Allocated after request is performed. Only used
     * after persistent config creation. */ 
    void *tokenRequest;

    /* Boolean to specifiy state of authorization.
     * 
     * If no file location is specified cURL performs
     * a clientId-based authorization (x-tidal-token: {client_id}). 
     * 
     * User-based and stream requests are disabled and result in a 401.
     * A 30s preview of a track (64kbps 22.05kHz MP3) or
     * a video (640x360 1116kbps MP4) can be requested.
     * This mode is primarly useful for metadata requests. */
    int demoEnabled;

    /* Constant pointers initialised with openTIDAL_SessionInit */
    const char *clientId;
    const char *clientSecret;
    const char *baseUrl;
    const char *authUrl;
   
    /* Pointers to allocated cJSON Values. Either from the persistent
     * session config, the tokenRequest or the refreshRequest. NULL if
     * demoEnabled is TRUE. */ 
    char *username;
    size_t userId;
    time_t expiresIn;
    char *countryCode;
    
    char *accessToken;
    char *refreshToken;

    char *audioQuality;
    char *videoQuality;

    /* Typecasted CURL Structure. Persistent cURL request handle. */
    void *curlHandle;
} openTIDAL_SessionContainer;

/* Initialised with openTIDAL_StructInit({pointer to container}). 
 * See structCtrl.c for more detailed documentation. */
typedef struct openTIDAL_ContentContainer {
    openTIDAL_LoginCodeContainer code;
    openTIDAL_LoginTokenContainer token;
    
    openTIDAL_UserContainer user;
    openTIDAL_UserSubscriptionContainer subscription;

    /* Pointer to dynamic arrays. Allocated one by one only if
     * needed. The arrays can also function as content pools in
     * case of module based endpoints (home endpoint). In this case
     * a direct position to the content is given. 
     * Each array has it's own identifier to retrieve the correct capacity
     * and total items in array values. 
     * The arrays are populated with helper functions (openTIDAL_StructAdd*)
     * and grow automatically heap by doubling it's current size. This prevents
     * the expensive use of realloc.
     * The initial capacity is 10.
     * To deallocate all arrays inside the ContentContainer call
     * openTIDAL_StructDelete({pointer to container}). */
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

    /* Custom status of performed request. Error handling of the tidalapi and
     * primitive http response codes are parsed to provide detailed information
     * about the state of the request. */
    int status;

    /* This static array is being used to store the capacity of each array.
     * The capacity array keeps track of the allocated heap size of
     * each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int capacity[8];
    /* This static array is being used to store the current number of items
     * added to each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int total[8];
   
    /* Pointer to allocated cJSON Structure. After a request is performed and
     * is successful it's allocated buffer is parsed with cJSON and deallocated. 
     * This pointer is used for deallocation purposes only. */
    void *json;
    /* Pointer to allocated cJSON Structure. After a get stream request is performed
     * and is successful it's stream manifest gets decoded with base64 and pased with
     * cJSON. 
     * This pointer is used for deallocation purposes only. */
    void *jsonManifest;
} openTIDAL_ContentContainer;

/* initialize openTIDAL with persistent config / specify location for config created by OAuth */
const int openTIDAL_SessionInit(openTIDAL_SessionContainer *session, const char *file_location);

/* enable verbose mode stdout (enables cURL verbose and openTIDAL verbose) */
void openTIDAL_Verbose(int loglevel);
void openTIDAL_ParseVerbose(const char *prefix, const char *string, int loglevel);
int openTIDAL_GetLogLevel();


/* Helper functions */
int openTIDAL_StringHelper(char **str, char* format, ...) __attribute__ ((format (printf,2,3)));

/* Memory management of main struct (Heap Allocation & Deallocation)
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


openTIDAL_ContentContainer
openTIDAL_GetUser(openTIDAL_SessionContainer *session);

openTIDAL_ContentContainer
openTIDAL_GetUserSubscription(openTIDAL_SessionContainer *session);


/* Playlist Endpoints */


openTIDAL_ContentContainer
openTIDAL_GetPlaylist(openTIDAL_SessionContainer *session, const char *playlistid);

openTIDAL_ContentContainer
openTIDAL_GetFavoritePlaylists(openTIDAL_SessionContainer *session, const int limit,
    const int offset, const char *order, const char *orderDirection);

openTIDAL_ContentContainer
openTIDAL_GetPlaylistItems(openTIDAL_SessionContainer *session,
        const char *playlistid, const int limit, const int offset);


/* Album Endpoints */


openTIDAL_ContentContainer
openTIDAL_GetAlbum(openTIDAL_SessionContainer *session, 
        const size_t albumid);

openTIDAL_ContentContainer
openTIDAL_GetAlbumItems(openTIDAL_SessionContainer *session,
        const size_t albumid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetFavoriteAlbums(openTIDAL_SessionContainer *session, 
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

int openTIDAL_AddFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid);

int openTIDAL_DeleteFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid);


/* Track Endpoints  */


openTIDAL_ContentContainer
openTIDAL_GetTrackContributors(openTIDAL_SessionContainer *session, 
        const size_t trackid, const int limit, const int offset);

openTIDAL_ContentContainer
openTIDAL_GetTrackMix(openTIDAL_SessionContainer *session, const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetTrackStream(openTIDAL_SessionContainer *session, const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetTrack(openTIDAL_SessionContainer *session, const size_t trackid);

openTIDAL_ContentContainer
openTIDAL_GetFavoriteTracks(openTIDAL_SessionContainer *session, const int limit,
        const int offset, const char *order, const char *orderDirection);


/* Video Endpoints  */


openTIDAL_ContentContainer openTIDAL_GetVideo(openTIDAL_SessionContainer *session,
        const size_t videoid);

openTIDAL_ContentContainer openTIDAL_GetFavoriteVideos(openTIDAL_SessionContainer *session,
        const int limit, const int offset, const char *order, const char *orderDirection);

openTIDAL_ContentContainer openTIDAL_GetVideoContributors(openTIDAL_SessionContainer *session,
        const size_t videoid, const int limit, const int offset);

openTIDAL_ContentContainer openTIDAL_GetVideoStream(openTIDAL_SessionContainer *session,
        const size_t videoid);


/* Mix Endpoints    */


openTIDAL_ContentContainer
openTIDAL_GetMixItems(openTIDAL_SessionContainer *session,
        const char *mixid);

openTIDAL_ContentContainer
openTIDAL_GetFavoriteMixes(openTIDAL_SessionContainer *session);


/* Search Endpoints */


openTIDAL_ContentContainer openTIDAL_SearchAll(openTIDAL_SessionContainer *session, char *term, const int limit);


/* Page Endpoints */


openTIDAL_ContentContainer openTIDAL_GetHome(openTIDAL_SessionContainer *session);
#ifdef __cplusplus
}
#endif

#endif

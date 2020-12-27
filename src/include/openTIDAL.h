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

typedef struct openTIDAL_LoginCodeContainer openTIDAL_LoginCodeContainer;
typedef struct openTIDAL_LoginTokenContainer openTIDAL_LoginTokenContainer; 
typedef struct openTIDAL_UserContainer openTIDAL_UserContainer;
typedef struct openTIDAL_UserSubscriptionContainer openTIDAL_UserSubscriptionContainer;
typedef struct openTIDAL_PlaylistContainer openTIDAL_PlaylistContainer;
typedef struct openTIDAL_ItemsContainer openTIDAL_ItemsContainer;
typedef struct openTIDAL_AlbumContainer openTIDAL_AlbumContainer;
typedef struct openTIDAL_CreditsContainer openTIDAL_CreditsContainer;
typedef struct openTIDAL_ArtistContainer openTIDAL_ArtistContainer;
typedef struct openTIDAL_LinkContainer openTIDAL_LinkContainer;
typedef struct openTIDAL_ETagContainer openTIDAL_ETagContainer;
typedef struct openTIDAL_MixContainer openTIDAL_MixContainer;
typedef struct openTIDAL_ContributorContainer openTIDAL_ContributorContainer;
typedef struct openTIDAL_StreamContainer openTIDAL_StreamContainer;

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

typedef struct openTIDAL_SessionContainer openTIDAL_SessionContainer;
/* main structure with dynamic arrays (See memory management of main struct) */
typedef struct openTIDAL_ContentContainer openTIDAL_ContentContainer;

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

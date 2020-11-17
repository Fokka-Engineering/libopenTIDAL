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

#include "cJSON.h"
#include "models.h"
#include "json_models.h"

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

char *get_playlist_etag(char *playlistid);
int delete_playlist(char *playlistid);
int delete_playlist_item(char *playlistid, size_t index, char *eTagHeader);
int move_playlist_item(char *playlistid, size_t index, size_t toIndex, char *eTagHeader);
int add_playlist_item(char *playlistid, size_t trackid, char *onDupes, char *eTagHeader);
int add_playlist_items(char *playlistid, char *trackids, char *onDupes, char *eTagHeader);
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

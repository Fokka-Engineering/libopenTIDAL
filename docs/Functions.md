###### openTIDAL Persistent

``` c
void init(char *file_location);
void init_demo();

void cleanup();
```

###### oAuth2

``` c
login_code_model login_create_code();
login_token_model login_create_token(char *device_code);
login_token_model login_refresh_token(char *refresh_token);

int logout();
```

###### User

``` c
user_model get_user();
user_subscription_model get_user_subscription();
```

###### Playlist

``` c
playlist_model get_playlist(char *playlistid);
playlist_model get_favorite_playlist(size_t limit, size_t offset, char *order, char *orderDirection);
items_model get_playlist_items(char *playlistid, size_t limit, size_t offset);
char *get_playlist_etag(char *playlistid);

playlist_model create_playlist(char *title, char *description);

int delete_playlist(char *playlistid);
int delete_playlist_item(char *playlistid, size_t index, char *eTagHeader);
int add_playlist_item(char *playlistid, size_t trackid, char *onDupes, char *eTagHeader);
int add_playlist_items(char *playlistid, char *trackids, char *onDupes, char *eTagHeader);
int move_playlist_item(char *playlistid, size_t index, size_t toIndex, char *eTagHeader);

int add_favorite_playlist(char *playlistid);
int delete_favorite_playlist(char *playlistid);
```

###### Album

```c
album_model get_album(size_t albumid);
items_model get_album_items(size_t albumid, size_t limit, size_t offset);
album_model get_favorite_album(size_t limit, size_t offset, char *order, char *orderDirection);

int add_favorite_album(size_t albumid);
int delete_favorite_album(size_t albumid);
```

###### Artist

```c
artist_model get_artist(size_t artistid);
artist_model get_favorite_artist(size_t limit, size_t offset, char *order, char *orderDirection);
artist_link_model get_artist_link(size_t artistid, size_t limit, size_t offset);
mix_model get_artist_mix(size_t artistid);
items_model get_artist_toptracks(size_t artistid, size_t limit, size_t offset);
items_model get_artist_videos(size_t artistid, size_t limit, size_t offset);
album_model get_artist_albums(size_t artistid, size_t limit, size_t offset);

int add_favorite_artist(size_t artistid);
int delete_favorite_artist(size_t artistid);
```

###### Track

```c
items_model get_track(size_t trackid);
items_model get_favorite_tracks(size_t limit, size_t offset, char *order, char *orderDirection);
contributor_model get_track_contributors(size_t trackid, size_t limit, size_t offset);
mix_model get_track_mix(size_t trackid);
stream_model get_track_stream(size_t trackid);

int delete_favorite_track(size_t trackid);
int add_favorite_track(size_t trackid);
```

###### Video

```c
items_model get_video(size_t videoid);
items_model get_favorite_videos(size_t limit, size_t offset, char *order, char *orderDirection);
contributor_model get_video_contributors(size_t videoid, size_t limit, size_t offset);
stream_model get_video_stream(size_t videoid);

int add_favorite_video(size_t videoid);
int delete_favorite_video(size_t videoid);
```

###### Mix

```c
items_model get_mix_items(char *mixid);
page_mix_model get_favorite_mixes();
```

###### Search

```c
search_model get_search(char *term, size_t limit);
```
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
#include "openTIDAL.h"
#include "handles.h"
#include "json_models.h"

cJSON *json_parse(char * input);
void parse_double(cJSON *object, double *number);
void parse_number(cJSON *object, size_t *number);
void parse_bool(cJSON *object, size_t *number);
void parse_string(cJSON *object, char **string);

int parse_status(cJSON *input_json, curl_model Value, const size_t id, const char *uuid);
int parse_unauthorized(cJSON *input_json, const size_t id);
int parse_notfound(cJSON *input_json, const size_t id, const char *uuid);
int parse_preconditionfailed(cJSON *input_json, const size_t id, const char *uuid);
int parse_badrequest(cJSON *input_json, const size_t id, const char *uuid);

/* parse Objects */
json_login_code_model json_parse_login_code(cJSON *directObject);
json_login_token_model json_parse_login_token(cJSON *directObject);
json_user_model json_parse_user(cJSON *directObject);
json_user_subscription_model json_parse_user_subscription(cJSON *directObject);
json_playlist_model json_parse_playlist(cJSON *directObject);
json_items_model json_parse_items(cJSON *directObject);
json_album_model json_parse_album(cJSON *directObject);
json_artist_model json_parse_artist(cJSON *directObject);
json_links_model json_parse_links(cJSON *directObject);
json_credit_model json_parse_credits(cJSON *directObject);
json_contributor_model json_parse_contributors(cJSON *directObject);
json_mix_model json_parse_mix(cJSON *directObject);
json_page_mix_model json_parse_page_mix(cJSON *directObject);
json_stream_model json_parse_stream(cJSON *directObject);
json_manifest_model json_parse_manifest(cJSON *directObject);

/* parse Values */
login_code_model parse_login_code_values(json_login_code_model json);
login_token_model parse_login_token_values(json_login_token_model json);
user_model parse_user_values(json_user_model json);
user_subscription_model parse_user_subscription_values(json_user_subscription_model json);
playlist_model parse_playlist_values(json_playlist_model json, size_t i);
items_model parse_items_values(json_items_model json, size_t i);
album_model parse_album_values(json_album_model json, size_t i);
artist_model parse_artist_values(json_artist_model json, size_t i);
artist_link_model parse_link_values(json_links_model json, size_t i);
contributor_model parse_contributor_values(json_contributor_model json, size_t i);
mix_model parse_mix_values(json_mix_model json);
page_mix_model parse_page_mix_values(json_page_mix_model json, size_t i);
stream_model parse_stream_values(json_stream_model json);

search_model parse_search(cJSON *input_json);

/* concatenation */
char *url_cat(const char *strOne, const size_t id, const char *strTwo, int appendCountryCode);
char *url_cat_str(const char *strOne, const char *id, const char *strTwo);

/* persistentJSON */
char *create_persistent_stream(char *username, char *audio_quality, char *video_quality);
void read_persistent_stream(cJSON *input_json);

/* encoding */
char *url_encode(const char *str);

/* persistent json  */
int scan_persistent();
void create_persistent(char *username, char *audio_quality, char *video_quality);
void refresh_persistent();

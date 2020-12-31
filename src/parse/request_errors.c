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

#include "../helper/helper.h"
#include "../openTIDAL.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

int
parse_status (cJSON *input_json, openTIDAL_CurlContainer *Value, const size_t id, const char *uuid)
{
    int status = 0;

    if (Value->responseCode == 400) {
        status = parse_badrequest (input_json, id, uuid);
    }
    else if (Value->responseCode == 401) {
        status = parse_unauthorized (input_json, id);
    }
    else if (Value->responseCode == 404) {
        status = parse_notfound (input_json, id, uuid);
    }
    else {
        status = 0;
    }

    return status;
}

int
parse_raw_status (long *code)
{
    int status;
    if (*code == 200 || *code == 204 || *code == 201) {
        status = 1;
    }
    else if (*code == 400) {
        status = -11;
        openTIDAL_VerboseHelper ("Request Error", "400: Bad Request (Invalid Indices)", 1);
    }
    else if (*code == 401) {
        status = -8;
        openTIDAL_VerboseHelper ("Request Error", "401: Unauthorized", 1);
    }
    else if (*code == 404) {
        status = -2;
        openTIDAL_VerboseHelper ("Request Error", "404: Resource not found", 1);
    }
    else if (*code == 412) {
        status = -4;
        openTIDAL_VerboseHelper ("Request Error", "412: Resource eTag invalid", 1);
    }
    else {
        status = 0;
    }
    return status;
}

int
parse_unauthorized (cJSON *input_json, const size_t id)
{
    int status = 0;
    const cJSON *subStatus = NULL;
    subStatus = cJSON_GetObjectItem (input_json, "subStatus");
    if (cJSON_IsNumber (subStatus) == 1) {
        if (subStatus->valueint == 4005) {
            status = -3;
            openTIDAL_VerboseHelper ("Request Error", "401: Asset %zu is not ready for playback", 1,
                                     id);
        }
        else if (subStatus->valueint == 6001) {
            status = -13;
            openTIDAL_VerboseHelper ("Request Error", "401: User does not have a valid session", 1);
        }
        else if (subStatus->valueint == 6005) {
            status = -7;
            openTIDAL_VerboseHelper ("Request Error", "401: Missing authorization header", 1);
        }
        else if (subStatus->valueint == 11002) {
            status = -6;
            openTIDAL_VerboseHelper ("Request Error", "401: Authorization header is invalid", 1);
        }
        else if (subStatus->valueint == 11003) {
            status = -5;
            openTIDAL_VerboseHelper ("Request Error",
                                     "401: The token has expired. (Expired on time)", 1);
        }
        else {
            status = 0;
            openTIDAL_VerboseHelper ("Request Error", "401: Unauthorized", 1);
        }
    }
    return status;
}

int
parse_notfound (cJSON *input_json, const size_t id, const char *uuid)
{
    int status = 0;
    const cJSON *subStatus = NULL;
    subStatus = cJSON_GetObjectItem (input_json, "subStatus");
    if (cJSON_IsNumber (subStatus) == 1) {
        if (subStatus->valueint == 2001) {
            status = -2;
            if (uuid == NULL) {
                openTIDAL_VerboseHelper ("Request Error", "404: Resource %zu not found", 1, id);
            }
            else {
                openTIDAL_VerboseHelper ("Request Error", "404: Resource %s not found", 1, uuid);
            }
        }
        else {
            openTIDAL_VerboseHelper ("Request Error", "404: Not Found", 1);
        }
    }
    return status;
}

int
parse_preconditionfailed (cJSON *input_json, const size_t id, const char *uuid)
{
    int status = 0;
    const cJSON *subStatus = NULL;
    subStatus = cJSON_GetObjectItem (input_json, "subStatus");
    if (cJSON_IsNumber (subStatus) == 1) {
        if (subStatus->valueint == 7002) {
            status = -4;
            if (uuid == NULL) {
                openTIDAL_VerboseHelper (
                    "Request Error", "412: If-None-Match (eTag) failed for Resource %zu", 1, id);
            }
            else {
                openTIDAL_VerboseHelper (
                    "Request Error", "412: If-None-Match (eTag) failed for Resource %s", 1, uuid);
            }
        }
        else {
            openTIDAL_VerboseHelper ("Request Error", "412: Precondition Failed", 1);
        }
    }
    return status;
}

int
parse_badrequest (cJSON *input_json, const size_t id, const char *uuid)
{
    int status = 0;
    const cJSON *subStatus = NULL;
    subStatus = cJSON_GetObjectItem (input_json, "subStatus");
    if (cJSON_IsNumber (subStatus) == 1) {
        if (subStatus->valueint == 1002) {
            status = -4;
            if (uuid == NULL) {
                openTIDAL_VerboseHelper ("Request Error", "400: Parameter missing for Resource %zu",
                                         1, id);
            }
            else {
                openTIDAL_VerboseHelper ("Request Error", "400: Parameter missing for Resource %s",
                                         1, uuid);
            }
        }
        else if (subStatus->valueint == 1005) {
            status = -12;
            if (uuid == NULL) {
                openTIDAL_VerboseHelper ("Request Error", "400: User not found for Resource %zu", 1,
                                         id);
            }
            else {
                openTIDAL_VerboseHelper ("Request Error", "400: User not found for Resource %s", 1,
                                         uuid);
            }
        }
        else if (subStatus->valueint == 6003) {
            status = -7;
            if (!uuid) {
                openTIDAL_VerboseHelper ("Request Error", "400: Token missing for Resource %zu", 1,
                                         id);
            }
            else {
                openTIDAL_VerboseHelper ("Request Error", "400: Token missing for Resource %s", 1,
                                         uuid);
            }
        }
        else {
            openTIDAL_VerboseHelper ("Request Error", "400: Bad Request", 1);
        }
    }
    return status;
}

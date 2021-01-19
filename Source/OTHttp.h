/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL contributors

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

#ifndef OTHTTP__h
#define OTHTTP__h

#include "openTIDAL.h"

enum OTHttpTypes
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD
};

struct OTHttpContainer
{
    void *handle;
    enum OTHttpTypes *type;
    int httpOk;
    int isAuthRequest;
    int isDummy;
    long responseCode;
    char *response;
    char *entityTagHeader;
    char *endpoint;
    char *parameter;
    char *postData;
};

void OTHttpContainerInit (struct OTHttpContainer *const http);
void OTHttpRequest (struct OTSessionContainer *const session, struct OTHttpContainer *const http);
enum OTStatus OTHttpParseStatus (struct OTHttpContainer *const http);
char *OTHttpParseHeader (char *buffer, char *key);
#endif /* OTHTTP__h */

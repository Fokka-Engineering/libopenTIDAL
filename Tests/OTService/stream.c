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

/* OTService get tests
 */

#include "../../Source/openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>

/* ./stream {id} {isPreview} */
int
main (int argc, char *argv[])
{
    struct OTSessionContainer *session;
    struct OTContentStreamContainer *content;
    enum OTTypes type = CONTENT_STREAM_CONTAINER;
    session = OTSessionInit ();
    OTSessionVerbose (session, 1);
    int status = OTSessionLogin (session, "/Users/hugo/Desktop/persistent");
    if (status != 0)
        goto end;

    if (argc == 3)
        {
            content = OTServiceGetStream (session, "tracks", argv[1], atoi (argv[2]), NULL);
            if (content)
                {
                    printf ("Response Not NULL // Status %d\n", content->status);
                    if (content->status == SUCCESS)
                        {
                            struct OTJsonContainer *url = NULL;
                            struct OTJsonContainer *urls = NULL;

                            urls = OTJsonGetObjectItem (content->manifest, "urls");
                            if (OTJsonIsArray (urls))
                                url = OTJsonGetArrayItem (urls, 0);
                            printf ("Url: %s\n", OTJsonGetStringValue (url));
                        }
                }
            OTDeallocContainer (content, type);
        }
end:
    OTSessionCleanup (session);
    return 0;
}

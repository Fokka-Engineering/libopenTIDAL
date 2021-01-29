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

int
main (void)
{
    struct OTSessionContainer *session = NULL;
    struct OTContentContainer *content = NULL;

    /* Create a new libopenTIDAL session. */
    session = OTSessionInit ();
    if (!session)
        return -1;

    /* Specify the clientId and clientSecret. */
    if (!(OTSessionClientPair (session, "CLIENTID", "CLIENTSECRET") == 0))
        return -1;

    const int limit = 10;
    const int offset = 0;
    const char *id = "13479529";
    /* Request the artefacts in an album with the following parameters:
     * (session, prefix, suffix, id, limit, offset, threadHandle) */
    content = OTServiceGetStandard (session, "albums", "items", id, limit, offset, NULL);
    if (content)
        {
            if (content->status == SUCCESS)
                {
                    struct OTJsonContainer *items = NULL;
                    struct OTJsonContainer *item = NULL;

                    /* You might want to check for NaN. */
                    double totalNumberOfItems
                        = OTJsonGetObjectItemNumberValue (content->tree, "totalNumberOfItems");
                    printf ("TotalNumberOfItems: %d\n", (int)totalNumberOfItems);

                    items = OTJsonGetObjectItem (content->tree, "items");
                    OTJsonArrayForEach (item, items)
                    {
                        struct OTJsonContainer *innerItem = NULL;
                        innerItem = OTJsonGetObjectItem (item, "item");

                        char *title = OTJsonGetObjectItemStringValue (innerItem, "title");
                        if (title)
                            printf ("Title: %s\n", title);
                    }
                }
        }

    OTDeallocContainer (content, CONTENT_CONTAINER);
    OTSessionCleanup (session);
    return 0;
}

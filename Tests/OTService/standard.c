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
    struct OTSessionContainer *session;
    struct OTContentContainer *content;
    enum OTTypes type = CONTENT_CONTAINER;
    session = OTSessionInit ();

    content = OTServiceGetStandard (session, "albums", "items", "13479529", 10, 0, NULL);
    if (content)
        {
            printf ("Response Not NULL\n");
            if (content->status == SUCCESS)
                {
                    struct OTJsonContainer *totalNumberOfItems = NULL;
                    struct OTJsonContainer *items = NULL;
                    struct OTJsonContainer *item = NULL;
                    totalNumberOfItems = OTJsonGetObjectItem (content->tree, "totalNumberOfItems");
                    items = OTJsonGetObjectItem (content->tree, "items");
                    printf ("totalNumberOfItems: %d\n",
                            (int)OTJsonGetNumberValue (totalNumberOfItems));
                    printf ("arraySize: %d\n", OTJsonGetArraySize(items));
                    OTJsonArrayForEach (item, items)
                    {
                        struct OTJsonContainer *innerItem = NULL;
                        struct OTJsonContainer *title = NULL;
                        innerItem = OTJsonGetObjectItem (item, "item");
                        title = OTJsonGetObjectItem (innerItem, "title");
                        printf ("Title: %s\n", OTJsonGetStringValue (title));
                    } 
                }
        }

    OTDeallocContainer (content, &type);
    OTSessionCleanup (session);
    return 0;
}

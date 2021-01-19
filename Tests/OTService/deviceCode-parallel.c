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

/* OTSession tests
 */

#include "../../Source/openTIDAL.h"
#include <stdio.h>

int
main (void)
{
    struct OTSessionContainer *session;
    enum OTTypes type = CONTENT_CONTAINER;
    int i;
    session = OTSessionInit ();
    if (!session)
        return -1;

    //    if (!(OTSessionClientPair (session, "CLIENTID", "CLIENTSECRET") == 0))
    //       return -1;

    OTSessionVerbose (session, 1);
#pragma omp parallel for
    for (i = 0; i < 10; i++)
        {
            void *handle;
            struct OTContentContainer *content;
            /* Creating and closing a session for one
             * request is not efficient. This is only a test. */
            handle = OTHttpThreadHandleCreate ();
            content = OTServiceGetDeviceCode (session, handle);
            if (content)
                {
                    printf ("Response Not NULL\n");
                    if (content->status == SUCCESS)
                        {
                            struct OTJsonContainer *deviceCode = NULL;
                            deviceCode = OTJsonGetObjectItem (content->tree, "deviceCode");
                            printf ("DeviceCode: %s\n", OTJsonGetStringValue (deviceCode));
                        }
                }

            OTDeallocContainer (content, type);
            OTHttpThreadHandleCleanup (handle);
        }
    OTSessionCleanup (session);
    return 0;
}


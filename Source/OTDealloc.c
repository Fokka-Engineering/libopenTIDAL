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

/* Deallocate allocated memory
 */

#include "OTHelper.h"
#include "OTJson.h"
#include "openTIDAL.h"
#include <stdlib.h>

void
OTDeallocContainer (void *container, enum OTTypes type)
{
    if (container)
        {
            struct OTSessionContainer *session;
            struct OTContentContainer *singleContainer;
            struct OTContentStreamContainer *streamContainer;
            switch (type)
                {
                case SESSION_CONTAINER:
                    session = (struct OTSessionContainer *)container;
                    free (session->persistentFileLocation);
                    OTJsonDelete (session->tree);
                    OTJsonDelete (session->renewalTree);
                    free (session);
                    break;
                case CONTENT_CONTAINER:
                    singleContainer = (struct OTContentContainer *)container;
                    OTJsonDelete (singleContainer->tree);
                    free (singleContainer);
                    break;
                case CONTENT_STREAM_CONTAINER:
                    streamContainer = (struct OTContentStreamContainer *)container;
                    OTJsonDelete (streamContainer->tree);
                    OTJsonDelete (streamContainer->manifest);
                    free (streamContainer);
                    break;
                }
        }
}

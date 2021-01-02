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

#include "../src/openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
    openTIDAL_SessionContainer session;
    openTIDAL_Verbose (2);
    openTIDAL_SessionInit (&session, "/Users/hugo/Documents/oT-config");

    openTIDAL_DeletePlaylist (&session, "7079f26e-174c-4012-8c3e-1f27b8189596");
    // openTIDAL_DeletePlaylistItem (&session, "7079f26e-174c-4012-8c3e-1f27b8189596", 0);
    // openTIDAL_MovePlaylistItem (&session, "7079f26e-174c-4012-8c3e-1f27b8189596", 1, 2);
    openTIDAL_SessionCleanup (&session);
    return 0;
}

from cffi import FFI
import glob
import os
ffibuilder = FFI()

header_file = "openTIDAL/api.processed.h"

with open(header_file) as fh:
    header = fh.read()

ffibuilder.cdef(header)

# set_source() gives the name of the python extension module to
# produce, and some C source code as a string.  This C code needs
# to make the declarated functions, types and globals available,
# so it is often just the "#include".
ffibuilder.set_source("openTIDAL._openTIDAL",
    '#include "api.h"',
    include_dirs=['openTIDAL'],
    libraries=['openTIDAL'])

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)

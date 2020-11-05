from cffi import FFI
ffibuilder = FFI()

header_file = "/Users/admin/Documents/Git/openTIDAL/py/openTIDAL.processed.h"

with open(header_file) as fh:
    header = fh.read()

ffibuilder.cdef(header)

# set_source() gives the name of the python extension module to
# produce, and some C source code as a string.  This C code needs
# to make the declarated functions, types and globals available,
# so it is often just the "#include".
ffibuilder.set_source("_openTIDAL_cffi",
"""
     #include "include/openTIDAL.h"   // the C header of the library
     #include "include/cJSON.h"
     #include "include/base64.h"
     #include "include/models.h"
""",
     libraries=['openTIDAL'])   # library name, for the linker

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)

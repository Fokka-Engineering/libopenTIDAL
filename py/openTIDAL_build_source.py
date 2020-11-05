from cffi import FFI
import glob
ffibuilder = FFI()

header_file = "/Users/admin/Documents/Git/openTIDAL/py/openTIDAL.processed.h"
source_files = glob.glob("src/*.c")
source_files_endpoints = glob.glob("src/endpoints/*.c")
source_files_parse = glob.glob("src/parse/*.c")

for i in source_files_endpoints:
    source_files.append(i)
for i in source_files_parse:
    source_files.append(i)

print(source_files)

with open(header_file) as fh:
    header = fh.read()

ffibuilder.cdef(header)

# set_source() gives the name of the python extension module to
# produce, and some C source code as a string.  This C code needs
# to make the declarated functions, types and globals available,
# so it is often just the "#include".
ffibuilder.set_source("_openTIDAL_cffi",
"""
     #include "src/include/openTIDAL.h"   // the C header of the library
     #include "src/include/cJSON.h"
     #include "src/include/base64.h"
     #include "src/include/models.h"
""",
     sources=source_files,
     libraries=['curl'])   # library name, for the linker

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)

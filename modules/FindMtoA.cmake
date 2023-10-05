#-*-cmake-*-
#
# yue.nicholas@gmail.com
#
# This auxiliary CMake file helps in find the MtoA headers and libraries
#
# MTOA_FOUND            set if MtoA is found.
# MTOA_INCLUDE_DIR      MtoA's include directory
# MTOA_mtoa_api_LIBRARY Full path location of libmtoa_api
# MTOA_LIBRARY          Alias for MTOA_mtoa_api_LIBRARY

find_package(PackageHandleStandardArgs)

##
## Obtain MtoA install location
##
set(MTOA_LOCATION ${MTOA_LOCATION})

set(MTOA_LIB ${MTOA_LOCATION}/lib)
set(MTOA_BIN ${MTOA_LOCATION}/bin)
set(MTOA_INCLUDE_DIR ${MTOA_LOCATION}/include)

message(STATUS "MTOA location: ${MTOA_LOCATION}")
message(STATUS "MTOA BIN:      ${MTOA_BIN}")
message(STATUS "MTOA LIB:      ${MTOA_LIB}")
message(STATUS "MTOA include:  ${MTOA_INCLUDE_DIR}")

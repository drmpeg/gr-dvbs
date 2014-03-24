INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DVBS dvbs)

FIND_PATH(
    DVBS_INCLUDE_DIRS
    NAMES dvbs/api.h
    HINTS $ENV{DVBS_DIR}/include
        ${PC_DVBS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DVBS_LIBRARIES
    NAMES gnuradio-dvbs
    HINTS $ENV{DVBS_DIR}/lib
        ${PC_DVBS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DVBS DEFAULT_MSG DVBS_LIBRARIES DVBS_INCLUDE_DIRS)
MARK_AS_ADVANCED(DVBS_LIBRARIES DVBS_INCLUDE_DIRS)


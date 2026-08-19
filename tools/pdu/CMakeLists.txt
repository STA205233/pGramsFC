cmake_minimum_required(VERSION 3.6)

# CMAKE_MINIMUM_REQUIRED_VERSION(VERSION 3.14)

# ## Initial definition of cmake variables
set(CMAKE_INSTALL_PREFIX $ENV{HOME} CACHE PATH "install prefix")

# set(CMAKE_BUILD_TYPE Release CACHE STRING "build type")
set(CMAKE_BUILD_TYPE Debug CACHE STRING "build type")
set(CMAKE_CXX_FLAGS_DEBUG "-g -W -Wall" CACHE STRING "CXX_FLAGS for debug")
set(CMAKE_C_FLAGS_DEBUG "-g -W -Wall" CACHE STRING "C_FLAGS for debug")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -W -Wall" CACHE STRING "CXX_FLAGS for release")
set(CMAKE_C_FLAGS_RELEASE "-O3 -W -Wall" CACHE STRING "C_FLAGS for release")
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ## Definition of project
project(GRAMSBalloon CXX C)
set(PROJECT_VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(MY_LIBRARY pdu_test)
find_path(D2XX_INC_DIR
  NAMES ftd2xx.h REQUIRED
  PATHS /usr/local/include)
find_path(WIN_TYPES_INC_DIR
  NAMES WinTypes.h REQUIRED
  PATHS /usr/local/include)
find_library(D2XX_LIB
  NAMES libftd2xx.so libftd2xx.dylib REQUIRED
  PATHS /usr/local/lib)
set(D2XX_INCLUDE_DIRS ${D2XX_INC_DIR} ${WIN_TYPES_INC_DIR})
message("-- D2XX_INCLUDE_DIRS: ${D2XX_INCLUDE_DIRS}")
message("-- D2XX_LIB: ${D2XX_LIB}")
include_directories(${D2XX_INCLUDE_DIRS})

find_path(VERSALOGIC_INC_DIR
  NAMES VL_OSALib.h REQUIRED
  PATHS /usr/local/include)
find_library(VERSALOGIC_LIB
  NAMES libVL_OSALib.so libVL_OSALib.dylib REQUIRED
  PATHS /usr/local/lib)
message(" -- VERSALOGIC_INC_DIR: ${VERSALOGIC_INC_DIR} ")
message(" -- VERSALOGIC_LIB: ${VERSALOGIC_LIB} ")

# MCP2210
if (GB_USE_MCP2210)
  find_library(MCP2210_LIB NAMES mcp2210hid_shared REQUIRED HINTS ${CMAKE_PREFIX_PATH}/bin)
  find_path(MCP2210_INC_DIR NAMES mcp2210_api.h REQUIRED HINTS ${CMAKE_PREFIX_PATH}/lib/inc)
  message("-- MCP2210_LIB: ${MCP2210_LIB}")
  message("-- MCP2210_INC_DIR: ${MCP2210_INC_DIR}")
else()
  set(MCP2210_LIB "")
  set(MCP2210_INC_DIR "")
endif(GB_USE_MCP2210)

include_directories(
  ${MPSSE_INC_DIR}
  ${VERSALOGIC_INC_DIR}
  ${MCP2210_INC_DIR}
)
add_executable(${MY_LIBRARY} pdu_test.cc)
target_link_libraries(${MY_LIBRARY}
  GBCore
  ${D2XX_LIB}
  ${VERSALOGIC_LIB}
  ${MCP2210_LIB}
)
target_link_directories(${MY_LIBRARY} PRIVATE
  $ENV{HOME}/lib
  $ENV{HOME}/lib/GRAMSBalloon
  $ENV{HOME}/lib/GRAMSBalloon/modules
  $ENV{HOME}/lib/GRAMSBalloon/core
)
target_include_directories(${MY_LIBRARY} PRIVATE
  $ENV{HOME}/include/GRAMSBalloon
  ${MPSSE_INC_DIR}
  ${D2XX_INCLUDE_DIRS}
)

add_executable(pdu_read pdu_read.cc)
target_link_libraries(pdu_read
  GBCore
  ${D2XX_LIB}
  ${VERSALOGIC_LIB}
)
target_link_directories(pdu_read PRIVATE
  $ENV{HOME}/lib
  $ENV{HOME}/lib/GRAMSBalloon
  $ENV{HOME}/lib/GRAMSBalloon/modules
  $ENV{HOME}/lib/GRAMSBalloon/core
)
target_include_directories(pdu_read PRIVATE
  $ENV{HOME}/include/GRAMSBalloon
  ${MPSSE_INC_DIR}
  ${D2XX_INCLUDE_DIRS}
)

add_executable(enable_test enable_test.cc)
target_link_libraries(enable_test
  GBCore
  ${D2XX_LIB}
  ${VERSALOGIC_LIB}
)
target_link_directories(enable_test PUBLIC
  $ENV{HOME}/lib
  $ENV{HOME}/lib/GRAMSBalloon
  $ENV{HOME}/lib/GRAMSBalloon/modules
  $ENV{HOME}/lib/GRAMSBalloon/core
)
target_include_directories(enable_test PUBLIC
  $ENV{HOME}/include/GRAMSBalloon
  ${MPSSE_INC_DIR}
  ${D2XX_INCLUDE_DIRS}
)
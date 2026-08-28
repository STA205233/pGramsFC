cmake_minimum_required(VERSION 3.6)

# CMAKE_MINIMUM_REQUIRED_VERSION(VERSION 3.14)

# ## Initial definition of cmake variables
set(CMAKE_INSTALL_PREFIX $ENV{HOME} CACHE PATH "install prefix")

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
set(MY_LIBRARY test_tof_bias)
add_executable(${MY_LIBRARY} test_tof_bias.cc)


target_link_libraries(${MY_LIBRARY}
  GBCore
  ${D2XX_LIB}
  ${VERSALOGIC_LIB}
)
target_link_directories(${MY_LIBRARY} PRIVATE
  $ENV{HOME}/lib
  $ENV{HOME}/lib/GRAMSBalloon
  $ENV{HOME}/lib/GRAMSBalloon/modules
  $ENV{HOME}/lib/GRAMSBalloon/core
)

target_include_directories(${MY_LIBRARY} PRIVATE
  $ENV{HOME}/include/GRAMSBalloon
)
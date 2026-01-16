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

# ## External libraries
# ## BOOST ###
find_package(Boost 1.60.0 REQUIRED CONFIG)
set(BOOST_INC_DIR ${Boost_INCLUDE_DIRS})
set(BOOST_LIB_DIR ${Boost_LIBRARY_DIRS})
set(BOOST_LIB ${Boost_LIBRARIES})
message("-- BOOST_INC_DIR: ${BOOST_INC_DIR}")
message("-- BOOST_LIB_DIR: ${BOOST_LIB_DIR}")
message("-- BOOST_LIB: ${BOOST_LIB}")
# add_executable(socket_test
# socket_test.cc
# )
add_executable(socket_server_test
  socket_server_test.cc
)

target_include_directories(socket_server_test
  PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(socket_server_test
  ${Boost_LIBRARIES}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(socket_server_test_async
  socket_server_test_async.cc
)
target_include_directories(socket_server_test_async
  PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  $ENV{HOME}/include/GRAMSBalloon
)
target_link_directories(socket_server_test_async
  PRIVATE
  ${Boost_LIBRARY_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  $ENV{HOME}/lib
)
target_link_libraries(socket_server_test_async
  ${Boost_LIBRARIES}
  ${CMAKE_CURRENT_SOURCE_DIR}
  GBCore
)

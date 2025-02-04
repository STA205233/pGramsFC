CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

project(GRAMSBalloonCommandSender CXX)
set(PROJECT_VERSION 1.0)
set(CMAKE_INSTALL_PREFIX $ENV{HOME} CACHE PATH "install prefix")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ANLNEXT_USE_READLINE True)

### boost
find_package(Boost 1.70.0 REQUIRED COMPONENTS system chrono thread)
set(BOOST_INC_DIR ${Boost_INCLUDE_DIRS})
set(BOOST_LIB_DIR ${Boost_LIBRARY_DIRS})
set(BOOST_LIB ${Boost_LIBRARIES})
message("-- BOOST_INC_DIR: ${BOOST_INC_DIR}")
message("-- BOOST_LIB_DIR: ${BOOST_LIB_DIR}")
message("-- BOOST_LIB: ${BOOST_LIB}")

if(NOT DEFINED ANLNEXT_INSTALL)
  if(DEFINED ENV{ANLNEXT_INSTALL})
    set(ANLNEXT_INSTALL $ENV{ANLNEXT_INSTALL})
  else()
    set(ANLNEXT_INSTALL $ENV{HOME})
  endif()
endif(NOT DEFINED ANLNEXT_INSTALL)
set(ANLNEXT_INC_DIR ${ANLNEXT_INSTALL}/include)
set(ANLNEXT_LIB_DIR ${ANLNEXT_INSTALL}/lib)
set(ANLNEXT_LIB ANLNext)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ANLNEXT_LIB_DIR}/anlnext)
message("-- ANLNEXT_INSTALL = ${ANLNEXT_INSTALL}")

### READLINE
if(ANLNEXT_USE_READLINE)
  set(READLINE_LIB readline)
  add_definitions(-DANLNEXT_USE_READLINE)
else(ANLNEXT_USE_READLINE)
  set(READLINE_LIB edit)
endif(ANLNEXT_USE_READLINE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquittopp REQUIRED)
set(MOSQUITTO_LIB PkgConfig::Mosquitto)
include_directories(
  include
  /opt/homebrew/include
  $ENV{HOME}/include
  ${Boost_INCLUDE_DIRS}
  ${READLINE_INC_DIR}
  ${ANLNEXT_INC_DIR}
  ../../onboard/source/core/include
  )

link_directories(
  ${READLINE_LIB_DIR}
  ${ANLNEXT_LIB_DIR}
  )

add_executable(send_command send_command.cc CommandSender.cc CommandBuilder.cc CommandSaver.cc ../../onboard/source/core/src/MosquittoIO.cc)
target_link_libraries(send_command ${BOOST_LIB} ${MOSQUITTO_LIB})
add_executable(dump_command dump_command.cc CommandBuilder.cc)
#add_executable(run_command_plan run_command_plan.cc CommandSender.cc CommandBuilder.cc CommandSaver.cc)

#target_link_libraries(run_command_plan
#  ${Boost_LIBRARIES}
#  ${READLINE_LIB}
#  ${ANLNEXT_LIB}
#  ${MOSQUITTO_LIB}
#  )

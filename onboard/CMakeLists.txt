cmake_minimum_required(VERSION 3.14)


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
set(USE_RUBY ON)
set(GB_SOCKET_CLIENT OFF)
set(MY_LIBRARY GRAMSBalloon)
set(cxx_definitions)

# set(RASPI ON)
find_package(Ruby 3.0 REQUIRED)

# ## options
# # library options
option(GB_USE_BAYCAT "enable BayCat " ON)
option(GB_USE_FT232H "enable FT232H" ON)
option(GB_USE_I2C ON)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(GB_USE_SYSTEM_MODULES ON)
else()
  set(GB_USE_SYSTEM_MODULES OFF)
endif()
message("-- GB_USE_SYSTEM_MODULES: ${GB_USE_SYSTEM_MODULES}")
option(GB_USE_HSQUICKLOOK "enable hsquicklook" OFF)
option(GB_APT_RUBY "installed ruby via apt" OFF)
option(GB_USE_MYSQL "enable mysql" OFF)
option(GB_USE_LJM "enable LJM library for LabJack devices" ON)

# install options
option(GB_INSTALL_HEADERS "install all header files" ON)
option(GB_INSTALL_CMAKE_FILES "install all cmake files" ON)
set(INSTALL_HEADERS ${GB_INSTALL_HEADERS})
set(INSTALL_CMAKE_FILES ${GB_INSTALL_CMAKE_FILES})

set(GB_PGRAMSBALLOON_PATH "../.." CACHE PATH "path to pGramsBalloon")
find_path(GB_PGRAMSBALLOON_INC_DIR NAMES CommunicationCodes.hh REQUIRED HINTS "${GB_PGRAMSBALLOON_PATH}/include" CMAKE_PREFIX_PATH)
message("-- Include file of pGRAMSBalloon Found")
message("-- GB_PGRAMSBALLOON_INC_DIR: ${GB_PGRAMSBALLOON_INC_DIR}")

if (GB_USE_BAYCAT OR GB_USE_FT232H)
  set(GB_USE_SPI ON)
else()
  set(GB_USE_SPI OFF)
endif()
  
# shortcut options
option(GB_GROUND "shortcut for ground system" OFF)

if(GB_GROUND)
message("-- GB_GROUND is set ")
set(GB_USE_FT232H OFF)
set(GB_USE_MYSQL ON)
set(GB_USE_SYSTEM_MODULES OFF)
set(GB_USE_LJM OFF)
set(GB_USE_BAYCAT OFF)
endif(GB_GROUND)


if (GB_USE_FT232H)
  add_compile_definitions("USE_FT232H")
else()
endif()

if (GB_USE_BAYCAT)
  set(GB_USE_I2C ON)
  set(GB_USE_VERSA_API ON)
  add_compile_definitions("USE_BAYCAT")
  add_compile_definitions("USE_VERSA_API")
else()
  set(GB_USE_VERSA_API OFF)
endif()

message("-- GB_USE_SPI: ${GB_USE_SPI}")
message("-- GB_USE_I2C: ${GB_USE_I2C}")
message("-- GB_USE_BAYCAT: ${GB_USE_BAYCAT}")

if(GB_USE_HSQUICKLOOK)
  add_compile_definitions("USE_HSQUICKLOOK")
endif(GB_USE_HSQUICKLOOK)

if(GB_USE_SYSTEM_MODULES)
  add_compile_definitions("USE_SYSTEM_MODULES")
endif(GB_USE_SYSTEM_MODULES)

# External libraries
# BOOST
find_package(Boost 1.60.0 REQUIRED CONFIG)
set(BOOST_INC_DIR ${Boost_INCLUDE_DIRS})
set(BOOST_LIB_DIR ${Boost_LIBRARY_DIRS})
set(BOOST_LIB ${Boost_LIBRARIES})
message("-- BOOST_INC_DIR: ${BOOST_INC_DIR} ")
message("-- BOOST_LIB_DIR: ${BOOST_LIB_DIR} ")
message("-- BOOST_LIB: ${BOOST_LIB} ")

# ANL
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
message("-- ANLNEXT_INSTALL: ${ANLNEXT_INSTALL}")

# SPI
if(GB_USE_FT232H)
  find_path(D2XX_INC_DIR
    NAMES ftd2xx.h REQUIRED
    PATHS /usr/local/include)
  find_path(WIN_TYPES_INC_DIR
    NAMES WinTypes.h REQUIRED
    PATHS /usr/local/include)
  find_library(D2XX_LIB
    NAMES libftd2xx.so libftd2xx.dylib REQUIRED
    PATHS /usr/local/lib)
  set(D2XX_LIB_DIR ${D2XX_LIB})
  set(D2XX_INCLUDE_DIRS ${D2XX_INC_DIR} ${WIN_TYPES_INC_DIR})
  message("-- D2XX_INCLUDE_DIRS: ${D2XX_INCLUDE_DIRS} ")
  message("-- D2XX_LIB: ${D2XX_LIB} ")
  include_directories(${D2XX_INCLUDE_DIRS})
  add_compile_definitions("USE_FT232H")
else()
  set(D2XX_LIB_DIR "")
  set(D2XX_INCLUDE_DIRS "")
  set(WIN_TYPES_INC_DIR "")
  set(D2XX_LIB "")
endif(GB_USE_FT232H)

# VersaLogic
if (GB_USE_VERSA_API)
  find_path(VERSALOGIC_INC_DIR
    NAMES VL_OSALib.h REQUIRED
    PATHS /usr/local/include CMAKE_PREFIX_PATH)
  find_library(VERSALOGIC_LIB
    NAMES libVL_OSALib.so libVL_OSALib.dylib REQUIRED
    PATHS /usr/local/lib CMAKE_PREFIX_PATH)
  add_compile_definitions("USE_VERSA_API")
  message("-- VERSALOGIC_INC_DIR: ${VERSALOGIC_INC_DIR} ")
  message("-- VERSALOGIC_LIB: ${VERSALOGIC_LIB} ")
else()
  set(VERSALOGIC_LIB "")
  set(VERSALOGIC_INC_LIB "")
endif(GB_USE_VERSA_API)

# I2C
if (GB_USE_I2C)
  set(BME_LIB GBBME68x)
  set(BME280_LIB GBBME280)
  set(BME_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BME68x_SensorAPI ${CMAKE_CURRENT_SOURCE_DIR}/BME280_SensorAPI)
  message("-- BME_LIB: ${BME_LIB}")
  message("-- BME280_LIB: ${BME280_LIB}")
  message("-- BME_INC_DIR: ${BME_INC_DIR}")
else()
  set(BME_LIB "")
  set(BME280_LIB "")
  set(BME_INC_DIR "")
endif(GB_USE_I2C)

# HSQUICKLOOK
if(GB_USE_HSQUICKLOOK)
  find_path(HSQUICKLOOK_INC_DIR
    NAMES hsquicklook/DocumentBuilder.hh
    PATHS $ENV{HSQUICKLOOK_INSTALL}/include /usr/local/include $ENV{HOME}/include)
  find_library(HSQUICKLOOK_LIB
    NAMES HSQuickLookAnalyzer
    PATHS $ENV{HSQUICKLOOK_INSTALL}/lib /usr/local/lib $ENV{HOME}/lib)
  message("-- HSQUICKLOOK_INC_DIR: ${HSQUICKLOOK_INC_DIR} ")
  message("-- HSQUICKLOOK_LIB: ${HSQUICKLOOK_LIB} ")

  find_package(mongocxx REQUIRED)
  set(MONGODB_LIB mongo::mongocxx_shared)
  message("-- MONGODB_LIB: ${MONGODB_LIB} ")
endif(GB_USE_HSQUICKLOOK)

# MYSQL
if(GB_USE_MYSQL)
  find_package(mysql-concpp REQUIRED CONFIG)
  set(MYSQL_INC_DIR ${MYSQL_CONCPP_INCLUDE_DIR})
  set(MYSQL_LIB_DIR ${MYSQL_CONCPP_RUNTIME_LIBRARY_DIR})
  set(MYSQL_LIB mysqlcppconnx)
  message("-- MYSQL_LIB_DIR: ${MYSQL_INC_DIR}")
  message("-- MYSQL_LIB_DIR: ${MYSQL_LIB_DIR}")
  message("-- MYSQL_LIB: ${MYSQL_LIB}")
  add_compile_definitions("USE_MYSQL")
else()
  set(MYSQL_INC_DIR "")
  set(MYSQL_LIB_DIR "")
  set(MYSQL_LIB "")
endif(GB_USE_MYSQL)

# Mosquitto
find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquittopp REQUIRED)
set(MOSQUITTO_LIB PkgConfig::Mosquitto)

# LJM
if (GB_USE_LJM)
  find_library(LJM_LIB NAMES LabJackM REQUIRED PATHS /usr/local/lib)
  find_path(LJM_INC_DIR NAMES LabJackM.h REQUIRED PATHS /usr/local/include)
  message(" -- LJM_LIB: ${LJM_LIB} ")
  message(" -- LJM_INC_DIR: ${LJM_INC_DIR} ")
  add_compile_definitions("USE_LJM")
else()
  set(LJM_LIB "")
  set(LJM_INC_DIR "")
endif(GB_USE_LJM)

# ## subdirectories
add_subdirectory(source)

if(USE_RUBY)
  add_subdirectory(rubyext)
endif(USE_RUBY)
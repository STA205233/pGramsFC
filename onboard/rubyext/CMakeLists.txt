########################################################
set(TARGET_EXT_LIBRARY GRAMSBalloon)
set(EXT_LIB_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/ruby)
set(SWIG_IF_FILE ${TARGET_EXT_LIBRARY}.i)
set(CLASS_LIST_FILE class_list_${TARGET_EXT_LIBRARY}.hh)
set(RUBY_EXT_INCLUDE_DIRS
  ../source/modules/include
  ../source/core/include
  ../source/demo/include
  ${ANLNEXT_INC_DIR}
  ${BOOST_INC_DIR}
  ${HSQUICKLOOK_INC_DIR}
  ${ROOT_INC_DIR}
  ${MYSQL_INC_DIR}
  )
set(RUBY_EXT_LIBRARY_DIRS
  /usr/local/lib
  ${ANLNEXT_LIB_DIR}
  ${CMAKE_INSTALL_PREFIX}/lib
  ${ROOT_LIB_DIR}
  ${MYSQL_LIB_DIR}
  )
set(RUBY_EXT_LIBRARIES
  ${ANLNEXT_LIB}
  GBCore
  GBModules
  ${HSQUICKLOOK_LIB}
  ${MONGODB_LIB}
  ${ROOT_LIB}
  ${MYSQL_LIB}
)


########################################################
if(GB_USE_SYSTEM_MODULES)
  list(APPEND cxx_definitions -DUSE_SYSTEM_MODULES)
endif()
if(GB_USE_HSQUICKLOOK)
  list(APPEND cxx_definitions -DUSE_HSQUICKLOOK)
endif()
if(GB_USE_ROOT)
  list(APPEND cxx_definitions -DUSE_ROOT)
endif()
if(GB_USE_MYSQL)
  list(APPEND cxx_definitions -DUSE_MYSQL)
endif()
if(GB_MAC)
  list(APPEND cxx_definitions -DGB_MAC)
endif()

set(SWIG_FLAGS ${cxx_definitions})
add_definitions(${cxx_definitions})

message("-- cxx_definitions: ${cxx_definitions}")

########################################################
include(CreateSwigRuby)

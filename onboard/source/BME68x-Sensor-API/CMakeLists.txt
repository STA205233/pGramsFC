cmake_minimum_required(VERSION 3.0)

include_directories(
  include
  ${ADD_INC_DIR}
  ${BOOST_INC_DIR}
  ${PIGPIO_INC_DIR}
  )

link_directories(
  ${BOOST_LIB_DIR}
  )

if(GB_USE_PIGPIO)
  set(GB_BME68x_CLASSES
    src/bme68x.c
    )
endif(GB_USE_PIGPIO)

add_library(${BME68x_LIB} SHARED
  ${GB_BME68x_CLASSES}
  )

target_link_libraries(GBBME68x
  ${ADD_LIB}
  ${BOOST_LIB}
  ${PIGPIO_LIB_DAEMON}
  ${PIGPIO_LIB_RT}
  )

install(TARGETS GBBME68x LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(INSTALL_HEADERS)
  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GRAMSBalloon
    FILES_MATCHING PATTERN "*.h")
endif(INSTALL_HEADERS)

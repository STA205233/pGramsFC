include_directories(
  include
  ${ADD_INC_DIR}
  ${BOOST_INC_DIR}
  )

link_directories(
  ${BOOST_LIB_DIR}
  )

set(GB_BME68x_CLASSES
  src/bme68x.c
  )

add_library(${BME_LIB} SHARED
  ${GB_BME68x_CLASSES}
  )

target_link_libraries(${BME_LIB}
  ${ADD_LIB}
  ${BOOST_LIB}
)
install(TARGETS ${BME_LIB} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(INSTALL_HEADERS)
  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GRAMSBalloon
    FILES_MATCHING PATTERN "*.h")
endif(INSTALL_HEADERS)

set(source_directory ${CMAKE_SOURCE_DIR}/BME68x_SensorAPI)
include_directories(
  ${source_directory}
)

set(GB_BME68x_CLASSES
  ${source_directory}/bme68x.c
)

add_library(${BME_LIB} SHARED
  ${GB_BME68x_CLASSES}
)

install(TARGETS ${BME_LIB} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(INSTALL_HEADERS)
  install(DIRECTORY ${source_directory}/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GRAMSBalloon
    FILES_MATCHING PATTERN "*.h")
endif(INSTALL_HEADERS)

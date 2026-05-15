set(source_directory ${CMAKE_SOURCE_DIR}/BME280_SensorAPI)
include_directories(
  ${source_directory}
)

set(GB_BME280_CLASSES
  ${source_directory}/bme280.c
)
  
add_library(${BME280_LIB} SHARED
  ${GB_BME280_CLASSES}
)
  
install(TARGETS ${BME280_LIB} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(INSTALL_HEADERS)
  install(DIRECTORY ${source_directory}/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GRAMSBalloon
    FILES_MATCHING PATTERN "*.h")
endif(INSTALL_HEADERS)

include_directories(
  include
  ${BOOST_INC_DIR}
  ${D2XX_INCLUDE_DIRS}
  ${MPSSE_INC_DIR}
  ${MYSQL_INC_DIR}
  ${GB_PGRAMSBALLOON_INC_DIR}
  ${VERSALOGIC_INC_DIR}
  )

link_directories(
  ${BOOST_LIB_DIR}
  ${MYSQL_LIB_DIR}
)

set(GB_CORE_CLASSES
  src/SerialCommunication.cc
  src/CommunicationFormat.cc
  src/CRC16.cc
  src/ErrorManager.cc
  src/DateManager.cc
  src/EncodedSerialCommunication.cc
  src/OrientationInformation.cc
  src/BaseTelemetryDefinition.cc
  src/HubHKTelemetry.cc
  src/VHKTelemetryMapping.cc
  src/MHADCMapping.cc
  src/IridiumPacketPool.cc
)

if(GB_USE_SPI)
  set(GB_SPI_CORE_CLASSES
    src/MAX31865IO.cc
    src/ADC128S102IO.cc
    src/DAC121S101IO.cc
  )
  if (GB_USE_BAYCAT_SPI)
    list(APPEND GB_SPI_CORE_CLASSES
      src/BayCatSPIIO.cc
    )
  endif(GB_USE_BAYCAT_SPI)
  if(GB_USE_FT232H)
    list(APPEND GB_SPI_CORE_CLASSES
      src/FT232HIO.cc
      src/MPSSEController.cc
      src/MPSSEDeviceManager.cc
    )
  endif(GB_USE_FT232H)
endif(GB_USE_SPI)



if(GB_USE_MYSQL)
  set(GB_MYSQL_CORE_CLASSES
    src/MySQLIO.cc
  )
endif(GB_USE_MYSQL)

set(GB_MOSQUITTO_CORE_CLASSES
    src/MosquittoIO.cc
  )
if (GB_SOCKET_CLIENT)
  set(GB_SOCKET_CLIENT_CORE_CLASSES
    src/SocketCommunicationClient.cc
  )
else(GB_SOCKET_CLIENT)
  set(GB_SOCKET_SERVER_CORE_CLASSES
    src/SocketCommunicationServer.cc
  )
endif(GB_SOCKET_CLIENT)

add_library(GBCore SHARED
  ${GB_CORE_CLASSES}
  ${GB_SPI_CORE_CLASSES}
  ${GB_MYSQL_CORE_CLASSES}
  ${GB_MOSQUITTO_CORE_CLASSES}
  ${GB_SOCKET_CLIENT_CORE_CLASSES}
  ${GB_SOCKET_SERVER_CORE_CLASSES}
  )

target_link_libraries(GBCore
  ${BOOST_LIB}
  ${D2XX_LIB}
  ${MPSSE_LIB}
  ${WAVEFORMS_LIB}
  ${ICM20948_LIB}
  ${MYSQL_LIB}
  ${MOSQUITTO_LIB}
  ${VERSALOGIC_LIB}
  )
target_compile_definitions(GBCore PUBLIC ${cxx_definitions})

install(TARGETS GBCore LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(INSTALL_HEADERS)
  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GRAMSBalloon
    FILES_MATCHING PATTERN "*.hh")
endif(INSTALL_HEADERS)

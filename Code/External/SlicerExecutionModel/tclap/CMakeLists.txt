project(TCLAP)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)


if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build with shared libraries." ON)
endif(NOT DEFINED BUILD_SHARED_LIBS)

include(GenerateTCLAPConfig.cmake)

configure_file(${TCLAP_SOURCE_DIR}/UseTCLAP.cmake.in
  ${TCLAP_BINARY_DIR}/UseTCLAP.cmake COPYONLY IMMEDIATE)


file(GLOB __files1 "${TCLAP_SOURCE_DIR}/include/tclap/*.h")
# Note "COPYING" is a filename containing the license file
install(FILES 
  ${__files1} 
  COPYING
  DESTINATION include/tclap COMPONENT Development
  )

install(FILES 
  ${TCLAP_BINARY_DIR}/UseTCLAP.cmake 
  ${TCLAP_BINARY_DIR}/install/TCLAPConfig.cmake 
  DESTINATION lib/tclap COMPONENT Development
  )

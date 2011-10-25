#---------------------------------------------
# CMake file for the CALATK project STILL CHANGING

cmake_minimum_required(VERSION 2.6)

#IF(COMMAND cmake_policy)
#  CMAKE_POLICY(SET CMP0003 NEW)
#ENDIF(COMMAND cmake_policy)


PROJECT(CALATK)

# Version setup
SET(CALATK_VERSION_MAJOR "0")
SET(CALATK_VERSION_MINOR "1")

MARK_AS_ADVANCED(Verbose_Compilation)
OPTION(Verbose_Compilation OFF)
IF(Verbose_Compilation)
  SET(CMAKE_VERBOSE_MAKEFILE ON)
ENDIF(Verbose_Compilation)

#
# Output path
#

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

#
# What to compile
#
OPTION(COMPILE_APPLICATIONS ON)
OPTION(BUILD_DOCUMENTATION ON)


#
# Include the fftw path
#
FIND_PATH(FFTW_PATH fftw3.h)
INCLUDE_DIRECTORIES(${FFTW_PATH})
FIND_LIBRARY(FFTW_LIB fftw3)

#
# Find ITK (used for file IO)
#
FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#
# Configure calatkConfigure.h.in
#
SET(CALATK_SYSTEM_UNIX ${UNIX})
SET(CALATK_SYSTEM_WINDOWS ${WIN32})
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/calatkConfigure.h.in ${CMAKE_BINARY_DIR}/calatkConfigure.h)

#
# Add OpenMP flag
#
if(WIN32)
  if(CMAKE_GENERATOR MATCHES Visual* )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Add custom c++ flag for openMP")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Add custom c++ flag for openMP")
  endif()
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" CACHE INTERNAL "Add custom c++ flag for openMP")
endif()

 

#
# Include directories
#
SET(CALATK_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Algorithms
  ${CMAKE_SOURCE_DIR}/Code/Libraries/DataStructures
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Utilities
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Numerics
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Metrics
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Kernels
  ${CMAKE_SOURCE_DIR}/Code/Libraries/States
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Resamplers
  ${CMAKE_SOURCE_DIR}/Code/Libraries/ObjectiveFunctions
  ${CMAKE_SOURCE_DIR}/Code/External/ITKTestingCommands
  ${CMAKE_BINARY_DIR}
  # Casey's Code
  ${CMAKE_SOURCE_DIR}/Utilities/DTIProcess/Library
)
INCLUDE_DIRECTORIES(${CALATK_INCLUDE_DIRS})


#
# Configure CALATKConfig.cmake.in and UseCALATK.cmake.in for external projects
#
SET(CALATK_INCLUDE_DIRS_CONFIG ${CALATK_INCLUDE_DIRS} ${ITK_INCLUDE_DIRS})
SET(CALATK_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH} ${ITK_LIBRARY_DIRS})
SET(CALATK_USE_FILE ${CMAKE_BINARY_DIR}/UseCALATK.cmake)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/UseCALATK.cmake.in
               ${CALATK_USE_FILE} COPYONLY IMMEDIATE)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/CALATKConfig.cmake.in
               ${CMAKE_BINARY_DIR}/CALATKConfig.cmake)


#
# Create the documentation
#
IF(BUILD_DOCUMENTATION)
  
  #INCLUDE( ${CMAKE_ROOT}/Modules/FindDoxygen.cmake)
  #CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
  #         ${CMAKE_CURRENT_SOURCE_DIR}/documents
  #         @ONLY IMMEDIATE)
  
  FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    ADD_CUSTOM_TARGET(doc doxygen ${CMAKE_BINARY_DIR}/Doxyfile)
  ENDIF(DOXYGEN_FOUND)   

  
ENDIF(BUILD_DOCUMENTATION)

# Set up for testing
enable_testing()
include(CTest)

#
# Go to Code
#
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Code)


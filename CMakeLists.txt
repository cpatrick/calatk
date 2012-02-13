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

# So that it finds nlopt
FIND_LIBRARY(NLOPT_LIB nlopt)

#
# Include the fftw path
#
FIND_PATH(FFTW_PATH fftw3.h)
INCLUDE_DIRECTORIES(${FFTW_PATH})
FIND_LIBRARY(FFTW_LIB fftw3)
FIND_LIBRARY(FFTWF_LIB fftw3f)

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
# Allow for boost support for some of the libraries (this is optional)
# 
OPTION(USE_BOOST "Use boost." OFF)
IF(USE_BOOST)
	FIND_PATH(BOOST_INCLUDE_DIR boost/config.hpp)
	FIND_PATH(BOOST_LIBRARY_DIR boost_date_time-vc71-mt.lib)
	
	LINK_DIRECTORIES(${BOOST_LIBRARY_DIR})
	INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIR})

	# Win32 has automatic linking of boost libraries so only add boost
	# libaries on unix
	IF(NOT WIN32 OR CYGWIN)
	  SET(BOOST_LIBRARIES boost_program_options)
	ENDIF(NOT WIN32 OR CYGWIN)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST")
ENDIF(USE_BOOST)

#
# Allow to choose the floating point type
#
OPTION(CHOOSE_FLOATING_POINT_TYPE "Choose floating point type on command line." ON)
IF(CHOOSE_FLOATING_POINT_TYPE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFLOATING_POINT_CHOICE")
ENDIF(CHOOSE_FLOATING_POINT_TYPE)

#
# Include directories
#
SET(CALATK_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Algorithms
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Base
  ${CMAKE_SOURCE_DIR}/Code/Libraries/DataStructures
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Utilities
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Numerics
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Metrics
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Kernels
  ${CMAKE_SOURCE_DIR}/Code/Libraries/States
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Resamplers
  ${CMAKE_SOURCE_DIR}/Code/Libraries/Interpolators
  ${CMAKE_SOURCE_DIR}/Code/Libraries/ObjectiveFunctions
  ${CMAKE_SOURCE_DIR}/Code/External/ITKTestingCommands
  ${CMAKE_SOURCE_DIR}/Code/External/jsoncpp
  ${CMAKE_SOURCE_DIR}/Code/External/libbfgs
  ${CMAKE_SOURCE_DIR}/Code/External/nlopt-2.2.4/api
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
include (CTest)

# Build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSEInstaller.txt")
set (CPACK_RESOURCE_FILE_README
     "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set (CPACK_RESOURCE_FILE_WELCOME
     "${CMAKE_CURRENT_SOURCE_DIR}/WELCOME.txt")

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Image Registration and Atlas-Building")
set (CPACK_PACKAGE_VENDOR "The CALATK Team")

set (CPACK_PACKAGE_VERSION_MAJOR "${CALATK_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${CALATK_VERSION_MINOR}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "CALATK - Cross-sectional and longitudinal atlas-building toolkit")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "calatk-${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

include (CPack)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCALATK_VERSION_MAJOR=${CALATK_VERSION_MAJOR} -DCALATK_VERSION_MINOR=${CALATK_VERSION_MINOR}" CACHE INTERNAL "Flags to support versioned json headers")

#
# Go to Code
#
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Code)


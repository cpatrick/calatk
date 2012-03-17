#---------------------------------------------
# CMake file for the CALATK project STILL CHANGING

cmake_minimum_required(VERSION 2.6)

#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)


project(CALATK)

# Version setup
set(CALATK_VERSION_MAJOR "0")
set(CALATK_VERSION_MINOR "1")

mark_as_advanced(Verbose_Compilation)
option(Verbose_Compilation OFF)
if(Verbose_Compilation)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif(Verbose_Compilation)

#
# Output path
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

#
# What to compile
#
option(COMPILE_APPLICATIONS "Compile command line applications." ON)
option(BUILD_DOCUMENTATION  "Build the documentation." OFF)


#
# Include the fftw path
#
find_path(FFTW_PATH fftw3.h)
include_directories(${FFTW_PATH})
find_library(FFTW_LIB fftw3)
find_library(FFTWF_LIB fftw3f)

#
# Find ITK (used for file IO)
#
find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
endif(ITK_FOUND)

#
# Shared libraries do not link correctly yet on Windows.
#
if(WIN32)
  set(BUILD_SHARED_LIBS OFF)
endif()

#
# Configure calatkConfigure.h.in
#
set(CALATK_SYSTEM_UNIX ${UNIX})
set(CALATK_SYSTEM_WINDOWS ${WIN32})
configure_file(${CMAKE_SOURCE_DIR}/calatkConfigure.h.in ${CMAKE_BINARY_DIR}/calatkConfigure.h)

#
# Add OpenMP flag
#
if(WIN32)
  if(CMAKE_GENERATOR MATCHES Visual* )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Add custom c++ flag for openMP")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Add custom c++ flag for openMP")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" CACHE INTERNAL "Add custom c++ flag for openMP")
endif()

#
# Allow for boost support for some of the libraries (this is optional)
# 
option(USE_BOOST "Use boost." OFF)
if(USE_BOOST)
	FIND_PATH(BOOST_INCLUDE_DIR boost/config.hpp)
	FIND_PATH(BOOST_LIBRARY_DIR boost_date_time-vc71-mt.lib)
	
	LINK_DIRECTORIES(${BOOST_LIBRARY_DIR})
	include_directories(${BOOST_INCLUDE_DIR})

	# Win32 has automatic linking of boost libraries so only add boost
	# libaries on unix
	if(NOT WIN32 OR CYGWIN)
	  set(BOOST_LIBRARIES boost_program_options)
	endif(NOT WIN32 OR CYGWIN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST")
endif(USE_BOOST)

#
# Allow to choose the floating point type
#
option(CHOOSE_FLOATING_POINT_TYPE "Choose floating point type on command line." ON)
if(CHOOSE_FLOATING_POINT_TYPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFLOATING_POINT_CHOICE")
endif(CHOOSE_FLOATING_POINT_TYPE)

#
# Include directories
#
set(CALATK_INCLUDE_DIRS
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
  ${CMAKE_BINARY_DIR}
  # Casey's Code
  ${CMAKE_SOURCE_DIR}/Utilities/DTIProcess/Library
)
include_directories(${CALATK_INCLUDE_DIRS})


#
# Configure CALATKConfig.cmake.in and UseCALATK.cmake.in for external projects
#
set(CALATK_INCLUDE_DIRS_CONFIG ${CALATK_INCLUDE_DIRS} ${ITK_INCLUDE_DIRS})
set(CALATK_LIBRARY_DIRS_CONFIG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${ITK_LIBRARY_DIRS})
set(CALATK_USE_FILE ${CMAKE_BINARY_DIR}/UseCALATK.cmake)

configure_file(${CMAKE_SOURCE_DIR}/UseCALATK.cmake.in
               ${CALATK_USE_FILE} COPYONLY IMMEDIATE)
configure_file(${CMAKE_SOURCE_DIR}/CALATKConfig.cmake.in
               ${CMAKE_BINARY_DIR}/CALATKConfig.cmake)


#
# Create the documentation
#
if(BUILD_DOCUMENTATION)
  #include( ${CMAKE_ROOT}/Modules/FindDoxygen.cmake)
  #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
  #         ${CMAKE_CURRENT_SOURCE_DIR}/documents
  #         @ONLY IMMEDIATE)
  
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    ADD_CUSTOM_TARGET(doc doxygen ${CMAKE_BINARY_DIR}/Doxyfile)
  endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)

# Set up for testing
enable_testing()
include (CTest)

# For Cppcheck.
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CALATK_SOURCE_DIR}/Scripts/Cppcheck" )

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCALATK_VERSION_MAJOR=${CALATK_VERSION_MAJOR} -DCALATK_VERSION_MINOR=${CALATK_VERSION_MINOR}" CACHE INTERNAL "Flags to support versioned json headers")

#
# Go to Code
#
add_subdirectory(${CMAKE_SOURCE_DIR}/Code)

#
# Configure CTestCustom.cmake.in
#
configure_file(${CALATK_SOURCE_DIR}/Scripts/CTestCustom.cmake.in
  ${CALATK_BINARY_DIR}/CTestCustom.cmake)


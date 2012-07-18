include( ExternalProject )

set( base "${CMAKE_BINARY_DIR}" )
set_property( DIRECTORY PROPERTY EP_BASE ${base} )

set( shared ON )
set( testing OFF )
set( build_type "Debug" )
if( CMAKE_BUILD_TYPE )
  set( build_type "${CMAKE_BUILD_TYPE}" )
endif()

set( CALATK_DEPENDS "" )

set( gen "${CMAKE_GENERATOR}" )

##
## Check if sytem ITK or superbuild ITK
##
if( NOT USE_SYSTEM_ITK )

  if( NOT GIT_EXECUTABLE )
    find_package( Git REQUIRED )
  endif( NOT GIT_EXECUTABLE )

  option( GIT_PROTOCOL_HTTP
    "Use HTTP for git access (useful if behind a firewall)" OFF )
  if( GIT_PROTOCOL_HTTP )
    set( GIT_PROTOCOL "http" CACHE STRING "Git protocol for file transfer" )
  else( GIT_PROTOCOL_HTTP )
    set( GIT_PROTOCOL "git" CACHE STRING "Git protocol for file transfer" )
  endif( GIT_PROTOCOL_HTTP )
  mark_as_advanced( GIT_PROTOCOL )

  ##
  ## Insight
  ##
  set( proj Insight )
  ExternalProject_Add( ${proj}
    GIT_REPOSITORY "${GIT_PROTOCOL}://itk.org/ITK.git"
    GIT_TAG "v4.2.0"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/Insight"
    BINARY_DIR Insight-Build
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
      -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${build_type}
      -DBUILD_SHARED_LIBS:BOOL=${shared}
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_TESTING:BOOL=OFF
      -DITK_USE_REVIEW:BOOL=ON
      -DUSE_FFTWF:BOOL=ON
      -DUSE_FFTWD:BOOL=ON
      -DUSE_SYSTEM_FFTW:BOOL=OFF
      -DITKGroup_IO:BOOL=ON
      -DITK_BUILD_ALL_MODULES:BOOL=OFF
    INSTALL_COMMAND ""
    )
  set( ITK_DIR "${base}/Insight-Build" )
  set( CALATK_DEPENDS ${CALATK_DEPENDS} "Insight" )
endif( NOT USE_SYSTEM_ITK )

##
## calatk - Normal Build
##
set( proj CALATK )
ExternalProject_Add( ${proj}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
  BINARY_DIR CALATK-Build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${build_type}
    -DBUILD_SHARED_LIBS:BOOL=${shared}
    -DBUILD_TESTING:BOOL=${testing}
    -DBUILD_DOCUMENTATION:BOOL=${BUILD_DOCUMENTATION}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCALATK_USE_SUPERBUILD:BOOL=FALSE
    -DITK_DIR:PATH=${ITK_DIR}
    -DFFTWF_LIB:FILEPATH=${CMAKE_BINARY_DIR}/Insight-Build/fftw/lib/libfftw3f.a
    -DFFTW_LIB:FILEPATH=${CMAKE_BINARY_DIR}/Insight-Build/fftw/lib/libfftw3.a
    -DFFTW_PATH:PATH=${CMAKE_BINARY_DIR}/Insight-Build/fftw/include
  INSTALL_COMMAND ""
  DEPENDS
    ${CALATK_DEPENDS}
  )

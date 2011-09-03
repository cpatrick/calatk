

#
# Include GenerateCLP
#
FIND_PACKAGE(GenerateCLP REQUIRED)
INCLUDE(${GenerateCLP_USE_FILE})

#
# Define a function for adding an application with an optional set of 
# libraries to link against (with GenerateCLP)
#
FUNCTION(add_clp_application appName mainFile libraries)
  
  # if we're coming from application off, force this application to compile
  IF(NOT COMPILE_APPLICATIONS)
    SET(force_application${appName} ON CACHE INTERNAL "temp variable for application switches" FORCE)
  ENDIF(NOT COMPILE_APPLICATIONS)
  
  # set up the option and mark it as advanced
  MARK_AS_ADVANCED( compile_${appName} )
  OPTION(compile_${appName} "Compile ${appName}" ON)
  
  # only turn the test on if COMPILE_APPLICATIONS was just turned on
  IF(COMPILE_APPLICATIONS)
    IF(force_application${appName})
      SET(compile_${appName} ON CACHE BOOL "Compile ${appName}" FORCE)
      SET(force_application${appName} OFF CACHE INTERNAL "temp variable for application switches" FORCE)
    ENDIF(force_application${appName})
    IF(compile_${appName})
  SET(CLP ${mainFile})
  SET( ${CLP}_SOURCE ${CLP}.cxx )
  GENERATECLP(${CLP}_SOURCE ${CLP}.xml "")
  
      # compile the program

  ADD_EXECUTABLE(${appName} ${CLP}.cxx)  
  TARGET_LINK_LIBRARIES( ${appName} CALATK ${libraries} ${ARGN} )
      
    ENDIF(compile_${appName})
  
  # if COMPILE_APPLICATIONS is off, force the test to turn on next time COMPILE_APPLICATIONS is turned on
  ELSE(COMPILE_APPLICATIONS)
    SET(compile_${appName} OFF CACHE BOOL "Compile ${appName}" FORCE)
  ENDIF(COMPILE_APPLICATIONS)

ENDFUNCTION(add_clp_application)


#
# Define a function for adding an application with an optional set of 
# libraries to link against (no GenerateCLP)
#
FUNCTION(add_application appName mainFile)
  
  # if we're coming from application off, force this application to compile
  IF(NOT COMPILE_APPLICATIONS)
    SET(force_application${appName} ON CACHE INTERNAL "temp variable for application switches" FORCE)
  ENDIF(NOT COMPILE_APPLICATIONS)
  
  # set up the option and mark it as advanced
  MARK_AS_ADVANCED( compile_${appName} )
  OPTION(compile_${appName} "Compile ${appName}" ON)
  
  # only turn the test on if COMPILE_APPLICATIONS was just turned on
  IF(COMPILE_APPLICATIONS)
    IF(force_application${appName})
      SET(compile_${appName} ON CACHE BOOL "Compile ${appName}" FORCE)
      SET(force_application${appName} OFF CACHE INTERNAL "temp variable for application switches" FORCE)
    ENDIF(force_application${appName})
    IF(compile_${appName})

      # compile the program
      ADD_EXECUTABLE(${appName} ${mainFile}.cxx )
      TARGET_LINK_LIBRARIES(${appName} CALATK ${ARGN})
      
    ENDIF(compile_${appName})
  
  # if COMPILE_APPLICATIONS is off, force the test to turn on next time COMPILE_APPLICATIONS is turned on
  ELSE(COMPILE_APPLICATIONS)
    SET(compile_${appName} OFF CACHE BOOL "Compile ${appName}" FORCE)
  ENDIF(COMPILE_APPLICATIONS)

ENDFUNCTION(add_application)



#
# Go to subdirectories
#

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/testme)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LDDMM)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TimeSeries)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LongitudinalAtlasBuilder)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilder)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FRATUtils)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/GeometricMetamorphosis)

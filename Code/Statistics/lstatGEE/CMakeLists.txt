PROJECT(lstatGEE)

#
# Add the lstatGEE application
#

IF(USE_BOOST)
  SET(libraries ITKCommon ITKAlgorithms ITKBasicFilters ITKIO ${BOOST_LIBRARIES})
ELSE(USE_BOOST)
  SET(libraries ITKCommon ITKAlgorithms ITKBasicFilters ITKIO)
ENDIF(USE_BOOST) 

# Need to manually set up CLP here, because we have multiple source files

SET(appName lstatGEE)
SET(mainFile lstatGEE)

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

    ADD_EXECUTABLE(${appName} ${CLP}.cxx surallfileM10Y06_MN_JP.cpp pxanalyse_MN.cpp GEEutil_MN.cpp)
    TARGET_LINK_LIBRARIES( ${appName} CALATK ${libraries})
      
  ENDIF(compile_${appName})
  
  # if COMPILE_APPLICATIONS is off, force the test to turn on next time COMPILE_APPLICATIONS is turned on
ELSE(COMPILE_APPLICATIONS)
  SET(compile_${appName} OFF CACHE BOOL "Compile ${appName}" FORCE)
ENDIF(COMPILE_APPLICATIONS)

#ADD_EXECUTABLE(tstMatrix tstMatrixLogExp.cxx)


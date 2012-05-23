option(COMPILE_CLP_MODULES "Build the Slicer CLP Modules" ON)
if(COMPILE_CLP_MODULES)
  #
  # Include everything to build generateclp
  #
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External/SlicerExecutionModel)

  #
  # Include GenerateCLP
  #
  find_package(GenerateCLP REQUIRED)
  include(${GenerateCLP_USE_FILE})

  #
  # Define a function for adding an application with an optional set of
  # libraries to link against (with GenerateCLP)
  #
  function(add_clp_application appName mainFile libraries)

    # if we're coming from application off, force this application to compile
    if(NOT COMPILE_APPLICATIONS)
      set(force_application${appName} ON CACHE INTERNAL "temp variable for application switches" FORCE)
    endif(NOT COMPILE_APPLICATIONS)

    # set up the option and mark it as advanced
    mark_as_advanced( compile_${appName} )
    option(compile_${appName} "Compile ${appName}" ON)

    # only turn the test on if COMPILE_APPLICATIONS was just turned on
    if(COMPILE_APPLICATIONS)
      if(force_application${appName})
        set(compile_${appName} ON CACHE BOOL "Compile ${appName}" FORCE)
        set(force_application${appName} OFF CACHE INTERNAL "temp variable for application switches" FORCE)
      endif(force_application${appName})

      if(compile_${appName})
        set(CLP ${mainFile})
        set( ${CLP}_SOURCE ${CLP}.cxx )
        GENERATECLP(${CLP}_SOURCE ${CLP}.xml "")

            # compile the program

        add_executable(${appName} ${CLP}.cxx)
        target_link_libraries( ${appName} CALATK ${libraries} ${IPOPT_LIBS} ${ARGN} )

        install( TARGETS ${appName}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib/static)
      endif(compile_${appName})

    # if COMPILE_APPLICATIONS is off, force the test to turn on next time COMPILE_APPLICATIONS is turned on
    else(COMPILE_APPLICATIONS)
      set(compile_${appName} OFF CACHE BOOL "Compile ${appName}" FORCE)
    endif(COMPILE_APPLICATIONS)

  endfunction(add_clp_application)
endif(COMPILE_CLP_MODULES)


#
# Define a function for adding an application with an optional set of
# libraries to link against (no GenerateCLP)
#
function(add_application appName mainFile)
  # if we're coming from application off, force this application to compile
  if(NOT COMPILE_APPLICATIONS)
    set(force_application${appName} ON CACHE INTERNAL "temp variable for application switches" FORCE)
  endif(NOT COMPILE_APPLICATIONS)

  # set up the option and mark it as advanced
  mark_as_advanced( compile_${appName} )
  option(compile_${appName} "Compile ${appName}" ON)

  # only turn the test on if COMPILE_APPLICATIONS was just turned on
  if(COMPILE_APPLICATIONS)
    if(force_application${appName})
      set(compile_${appName} ON CACHE BOOL "Compile ${appName}" FORCE)
      set(force_application${appName} OFF CACHE INTERNAL "temp variable for application switches" FORCE)
    endif(force_application${appName})
    if(compile_${appName})

      # compile the program
      add_executable(${appName} ${mainFile}.cxx )
      target_link_libraries(${appName} CALATK ${IPOPT_LIBS} ${ARGN})

      install( TARGETS ${appName}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

    endif(compile_${appName})

  # if COMPILE_APPLICATIONS is off, force the test to turn on next time COMPILE_APPLICATIONS is turned on
  else(COMPILE_APPLICATIONS)
    set(compile_${appName} OFF CACHE BOOL "Compile ${appName}" FORCE)
  endif(COMPILE_APPLICATIONS)

endfunction(add_application)

# First go to Libraries, then Applications
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Applications)

# Add utilities
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Utilities)

# Add the statistical code
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Statistics)

# Add external utilities
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External)

# Set up testing
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Testing)


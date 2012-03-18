
# Go to library subdirectories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/DataStructures)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Utilities)

set(CALATK_jsoncpp_source ${CMAKE_SOURCE_DIR}/Code/External/jsoncpp/jsoncpp.cpp)

# Create a single library for the toolkit
set(CALATK_source
  ${CALATK_Base_source}
  ${CALATK_Utilities_source}
  ${CALATK_DataStructures_source}
  ${CALATK_jsoncpp_source}
#  ${CALATK_Algorithms_source}
)

IF(USE_LBFGS)
  # Set up source for LBFGS
  set(CALATK_libbfgs_source ${CMAKE_SOURCE_DIR}/Code/External/libbfgs/lbfgs.c)
  set(CALATK_source
    ${CALATK_source}
    ${CALATK_libbfgs_source}
    )
ENDIF(USE_LBFGS)

add_library(CALATK ${CALATK_source})
target_link_libraries(CALATK ${FFTW_LIB} ${FFTWF_LIB} ${ITK_LIBRARIES} ${NLOPT_LIB} ${IPOPT_LIBS})

INSTALL( TARGETS CALATK 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

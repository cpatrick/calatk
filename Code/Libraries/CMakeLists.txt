
# Go to library subdirectories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/DataStructures)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Utilities)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../External/jsoncpp)


# Create a single library for the toolkit
set(CALATK_source
  ${CALATK_Utilities_source}
  ${CALATK_DataStructures_source}
  ${CALATK_jsoncpp_source}
#  ${CALATK_Algorithms_source}
)

add_library(CALATK ${CALATK_source})
target_link_libraries(CALATK ${FFTW_LIB} ITKCommon ITKIO itksys)


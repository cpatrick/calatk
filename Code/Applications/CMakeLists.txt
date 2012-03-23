#
# Go to subdirectories
#

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLine)

if(COMPILE_CLP_MODULES)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/testme)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/GeometricMetamorphosis)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LDDMMRelaxation)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LDDMMShooting)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LDDMM)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/MetamorphosisShooting)

  #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilder)

  #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TimeSeries)

  #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LongitudinalAtlasBuilder)

  #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilder)

  #add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FRATUtils)
endif(COMPILE_CLP_MODULES)

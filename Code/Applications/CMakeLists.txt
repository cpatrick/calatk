#
# Go to subdirectories
#

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLine)

if(COMPILE_CLP_MODULES)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/GeometricMetamorphosis)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LDDMM)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilder)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/MetamorphosisShooting)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LongitudinalAtlasBuilder)

endif(COMPILE_CLP_MODULES)

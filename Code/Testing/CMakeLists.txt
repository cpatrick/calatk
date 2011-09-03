
# Set bin and output dirs for tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(test_temp_dir ${FRAT_BINARY_DIR}/Testing/Temporary)
set(test_input_dir ${FRAT_SOURCE_DIR}/TestingData)

#
# Define a function for adding a test with an optional set of lbraries to link against
#
FUNCTION(add_test_executable testName)

  # if we're coming from testing off, force this test to compile
  IF(NOT BUILD_TESTING)
    SET(force_testing${testName} ON CACHE INTERNAL "temp variable for testing switches" FORCE)
  ENDIF(NOT BUILD_TESTING)

  # set up the option and mark it as advanced
  MARK_AS_ADVANCED( compile_${testName} )
  OPTION(compile_${testName} "Compile ${testName}" ON)

  # only turn the test on if BUILD_TESTING was just turned on
  IF(BUILD_TESTING)
    IF(force_testing${testName})
      SET(compile_${testName} ON CACHE BOOL "Compile ${testName}" FORCE)
      SET(force_testing${testName} OFF CACHE INTERNAL "temp variable for testing switches" FORCE)
    ENDIF(force_testing${testName})
    IF(compile_${testName})

      # compile the program
      ADD_EXECUTABLE(${testName} ${testName}.cxx )
      TARGET_LINK_LIBRARIES(${testName} ${ARGN})

    ENDIF(compile_${testName})

  # if BUILD_TESTING is off, force the test to turn on next time BUILD_TESTING is turned on
  ELSE(BUILD_TESTING)
    SET(compile_${testName} OFF CACHE BOOL "Compile ${testName}" FORCE)
  ENDIF(BUILD_TESTING)

ENDFUNCTION(add_test_executable)

#
# Define a function that adds a ctest test only if the executable is turned on
#
FUNCTION(add_ctest_test testName)
  IF(BUILD_TESTING AND compile_${testName})
    add_test(${testName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testName} ${ARGN})
  ENDIF(BUILD_TESTING AND compile_${testName})
ENDFUNCTION(add_ctest_test)


#------------------------------------------------------------------------------
# Add test executables without ctest tests
#

## conservationTest
add_test_executable(conservationTest FRAT ITKCommon ITKIO)

## FlowTest
add_test_executable(FlowTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## vvImageTest
add_test_executable(vvImageTest FRAT ITKCommon ITKIO)

## LDDMMTest
add_test_executable(LDDMMTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## GradientTest
add_test_executable(GradientTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## IOTest
add_test_executable(IOTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesTest
add_test_executable(TimeSeriesTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## LDDMMTestMultiscale
add_test_executable(LDDMMTestMultiscale FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesTestMultiscale
add_test_executable(TimeSeriesTestMultiscale FRAT ${FFTW_LIB} ITKCommon ITKIO)

## AtlasBuilderTest
add_test_executable(AtlasBuilderTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## AtlasBuilderTestMultiscale
add_test_executable(AtlasBuilderTestMultiscale FRAT ${FFTW_LIB} ITKCommon ITKIO)

## mapTest
add_test_executable(mapTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## AffineITKTest
add_test_executable(AffineITKTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesDataTest
add_test_executable(TimeSeriesDataTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesComputePoints
add_test_executable(TimeSeriesComputePoints FRAT ${FFTW_LIB} ITKCommon ITKIO)

## LongitudinalAtlasTest
add_test_executable(LongitudinalAtlasTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## resizeImage
add_test_executable(resizeImage FRAT ITKCommon ITKIO)

## imageMask
add_test_executable(imageMask FRAT ITKCommon ITKIO)

## TimeSeriesFinalEnergy
add_test_executable(TimeSeriesFinalEnergy FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesWarpImageToTime
add_test_executable(TimeSeriesWarpImageToTime FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesDetJacobian
add_test_executable(TimeSeriesDetJacobian FRAT ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesComputeMaps
add_test_executable(TimeSeriesComputeMaps FRAT ${FFTW_LIB} ITKCommon ITKIO)

## ComputeDetJacobian
add_test_executable(ComputeDetJacobian FRAT ${FFTW_LIB} ITKCommon ITKIO)

## GeometricMetamorphosis
add_test_executable(GeometricMetamorphosisTest FRAT ${FFTW_LIB} ITKCommon ITKIO)

## GeometricMetamorphosisFigures
add_test_executable(GeometricMetamorphosisFiguresTest FRAT ${FFTW_LIB} ITKCommon ITKIO)


#------------------------------------------------------------------------------
# Add test executables with ctest tests
#

## advectionTest
add_test_executable(advectionTest FRAT ${FFTW_LIB} ITKCommon ITKIO)
add_ctest_test(advectionTest ${test_temp_dir})

## VectorImage2DTest
add_test_executable(VectorImage2DTest FRAT ITKCommon ITKIO)
add_ctest_test(VectorImage2DTest)

## VectorImage3DTest
add_test_executable(VectorImage3DTest FRAT ITKCommon ITKIO)
add_ctest_test(VectorImage3DTest)

## convertITKMetaDataTest
add_test_executable(convertITKMetaDataTest FRAT ${FFTW_LIB} ITKCommon ITKIO)
add_ctest_test(convertITKMetaDataTest
  ${test_input_dir}/2Dsample.mha
  ${test_input_dir}/3Dsample.mha
  )

## writeFileITKTest
add_test_executable(writeFileITKTest FRAT ${FFTW_LIB} ITKCommon ITKIO)
add_ctest_test(writeFileITKTest
  # 2D input
  ${test_input_dir}/2Dsample.mha
  # 3D input
  ${test_input_dir}/3Dsample.mha
  # output dir
  ${test_temp_dir}
  )

## dirExistsTest
add_test_executable(dirExistsTest FRAT ITKCommon ITKIO)
add_ctest_test(dirExistsTest
  # A valid directory path
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

## makeDirIfNeededTest
add_test_executable(makeDirIfNeededTest FRAT ITKCommon ITKIO)
add_ctest_test(makeDirIfNeededTest
  # Temporary output path
  ${test_temp_dir}
  )

## removeDoubleSlashesTest
add_test_executable(removeDoubleSlashesTest FRAT ITKCommon ITKIO)
add_ctest_test(removeDoubleSlashesTest)


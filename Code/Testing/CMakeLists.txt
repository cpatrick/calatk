
# Set bin and output dirs for tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(test_temp_dir ${CALATK_BINARY_DIR}/Testing/Temporary)
set(test_input_dir ${CALATK_SOURCE_DIR}/TestingData)
set(test_bin_dir ${CALATK_BINARY_DIR}/Code/Testing)

SET(ALGORITHMS_TESTS_FILTERING ${test_bin_dir}/testDriverFiltering)
SET(ALGORITHMS_TESTS_FILTERING ${test_bin_dir}/testDriverPDESolver)

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

#
# Define a function that adds a ctest test with image comparison only if the executable is turned on
#
FUNCTION(add_ctest_test_image_compare testName testDriver imIn imOut imBaseline)
  IF(BUILD_TESTING AND compile_${testName})
    add_test(${testName} ${testDriver}
      --compare ${test_input_dir}/Baseline/${imBaseline}
                ${test_temp_dir}/${imOut}
      ${testName}
      ${test_input_dir}/${imIn}
      ${test_temp_dir}/${imOut}
      ${ARGN}
      )
  ENDIF(BUILD_TESTING AND compile_${testName})
ENDFUNCTION(add_ctest_test_image_compare)

#------------------------------------------------------------------------------
# New tests, added during the redevelopment of FRAT -> CALATK
#

# Set up source code files for all the tests which are processed by the testDriver

SET(testDriverFiltering_SRCS
calatkGaussianBlurringTest.cxx
)

SET(testDriverPDESolver_SRCS
calatkAdvectionTest.cxx
)

ADD_EXECUTABLE(testDriverFiltering testDriverFiltering.cxx ${testDriverFiltering_SRCS})
TARGET_LINK_LIBRARIES(testDriverFiltering CALATK ITKCommon ITKIO)

add_ctest_test_image_compare(calatkGaussianBlurringTest ${ALGORITHMS_TESTS_FILTERING} I0_short.nhdr I0_blurred.nrrd I0-blurred-sig01-baseline.nrrd)

ADD_EXECUTABLE(testDriverPDESolver testDriverPDESolver.cxx ${testDriverPDESolver_SRCS})
TARGET_LINK_LIBRARIES(testDriverPDESolver CALATK ITKCommon ITKIO)

add_ctest_test_image_compare(calatkAdvectionTest ${ALGORITHMS_TESTS_PDESOLVER} I0_short.nhdr I0_advected.nrrd I0-advected-baseline-x.nrrd 1 0 0.25)

add_ctest_test_image_compare(calatkAdvectionTest ${ALGORITHMS_TESTS_PDESOLVER} I0_short.nhdr I0_advected.nrrd I0-advected-baseline-y.nrrd 0 1 0.25)

add_ctest_test_image_compare(calatkAdvectionTest ${ALGORITHMS_TESTS_PDESOLVER} I0_short.nhdr I0_advected.nrrd I0-advected-baseline-diagonal.nrrd 1 1 0.25)

# Set up all the test which go through the standard (non-test-driver) testing mechanism

add_test_executable(calatkImageDataStructureTest CALATK ITKCommon ITKIO)
add_ctest_test(calatkImageDataStructureTest ${test_temp_dir})




#add_ctest_test(writeFileITKTest
#  # 2D input
#  ${test_input_dir}/2Dsample.mha
#  # 3D input
#  ${test_input_dir}/3Dsample.mha
#  # output dir
#  ${test_temp_dir}


#------------------------------------------------------------------------------
# Add test executables without ctest tests
#

## conservationTest
#add_test_executable(conservationTest CALATK ITKCommon ITKIO)

## FlowTest
#add_test_executable(FlowTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## vvImageTest
#add_test_executable(vvImageTest CALATK ITKCommon ITKIO)

## LDDMMTest
#add_test_executable(LDDMMTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## GradientTest
#add_test_executable(GradientTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## IOTest
#add_test_executable(IOTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesTest
#add_test_executable(TimeSeriesTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## LDDMMTestMultiscale
#add_test_executable(LDDMMTestMultiscale CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesTestMultiscale
#add_test_executable(TimeSeriesTestMultiscale CALATK ${FFTW_LIB} ITKCommon ITKIO)

## AtlasBuilderTest
#add_test_executable(AtlasBuilderTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## AtlasBuilderTestMultiscale
#add_test_executable(AtlasBuilderTestMultiscale CALATK ${FFTW_LIB} ITKCommon ITKIO)

## mapTest
#add_test_executable(mapTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## AffineITKTest
#add_test_executable(AffineITKTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesDataTest
#add_test_executable(TimeSeriesDataTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesComputePoints
#add_test_executable(TimeSeriesComputePoints CALATK ${FFTW_LIB} ITKCommon ITKIO)

## LongitudinalAtlasTest
#add_test_executable(LongitudinalAtlasTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## resizeImage
#add_test_executable(resizeImage CALATK ITKCommon ITKIO)

## imageMask
#add_test_executable(imageMask CALATK ITKCommon ITKIO)

## TimeSeriesFinalEnergy
#add_test_executable(TimeSeriesFinalEnergy CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesWarpImageToTime
#add_test_executable(TimeSeriesWarpImageToTime CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesDetJacobian
#add_test_executable(TimeSeriesDetJacobian CALATK ${FFTW_LIB} ITKCommon ITKIO)

## TimeSeriesComputeMaps
#add_test_executable(TimeSeriesComputeMaps CALATK ${FFTW_LIB} ITKCommon ITKIO)

## ComputeDetJacobian
#add_test_executable(ComputeDetJacobian CALATK ${FFTW_LIB} ITKCommon ITKIO)

## GeometricMetamorphosis
#add_test_executable(GeometricMetamorphosisTest CALATK ${FFTW_LIB} ITKCommon ITKIO)

## GeometricMetamorphosisFigures
#add_test_executable(GeometricMetamorphosisFiguresTest CALATK ${FFTW_LIB} ITKCommon ITKIO)


#------------------------------------------------------------------------------
# Add test executables with ctest tests
#

## advectionTest
#add_test_executable(advectionTest CALATK ${FFTW_LIB} ITKCommon ITKIO)
#add_ctest_test(advectionTest ${test_temp_dir})

## VectorImage2DTest
#add_test_executable(VectorImage2DTest CALATK ITKCommon ITKIO)
#add_ctest_test(VectorImage2DTest)

## VectorImage3DTest
#add_test_executable(VectorImage3DTest CALATK ITKCommon ITKIO)
#add_ctest_test(VectorImage3DTest)

## convertITKMetaDataTest
#add_test_executable(convertITKMetaDataTest CALATK ${FFTW_LIB} ITKCommon ITKIO)
#add_ctest_test(convertITKMetaDataTest
#  ${test_input_dir}/2Dsample.mha
#  ${test_input_dir}/3Dsample.mha
#  )

## writeFileITKTest
#add_test_executable(writeFileITKTest CALATK ${FFTW_LIB} ITKCommon ITKIO)
#add_ctest_test(writeFileITKTest
#  # 2D input
#  ${test_input_dir}/2Dsample.mha
#  # 3D input
#  ${test_input_dir}/3Dsample.mha
#  # output dir
#  ${test_temp_dir}
#  )

## dirExistsTest
#add_test_executable(dirExistsTest CALATK ITKCommon ITKIO)
#add_ctest_test(dirExistsTest
#  # A valid directory path
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  )

## makeDirIfNeededTest
#add_test_executable(makeDirIfNeededTest CALATK ITKCommon ITKIO)
#add_ctest_test(makeDirIfNeededTest
#  # Temporary output path
#  ${test_temp_dir}
#  )

## removeDoubleSlashesTest
#add_test_executable(removeDoubleSlashesTest CALATK ITKCommon ITKIO)
#add_ctest_test(removeDoubleSlashesTest)


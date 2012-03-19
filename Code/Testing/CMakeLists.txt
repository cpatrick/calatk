
# Set bin and output dirs for tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(test_temp_dir ${CALATK_BINARY_DIR}/Testing/Temporary)
set(test_input_dir ${CALATK_SOURCE_DIR}/TestingData)
set(test_bin_dir ${CALATK_BINARY_DIR}/Code/Testing)

set(ALGORITHMS_TESTS_FILTERING ${test_bin_dir}/testDriverFiltering)
set(ALGORITHMS_TESTS_PDESOLVER ${test_bin_dir}/testDriverPDESolver)
set(ALGORITHMS_TESTS_ALGORITHMS ${test_bin_dir}/testDriverAlgorithms)

#
# Define a function for adding a test with an optional set of libraries to link against
#
function(add_test_executable testName)

  # if we're coming from testing off, force this test to compile
  if(NOT BUILD_TESTING)
    set(force_testing${testName} ON CACHE INTERNAL "temp variable for testing switches" FORCE)
  endif(NOT BUILD_TESTING)

  # set up the option and mark it as advanced
  mark_as_advanced( compile_${testName} )
  option(compile_${testName} "Compile ${testName}" ON)

  # only turn the test on if BUILD_TESTING was just turned on
  if(BUILD_TESTING)
    if(force_testing${testName})
      set(compile_${testName} ON CACHE BOOL "Compile ${testName}" FORCE)
      set(force_testing${testName} OFF CACHE INTERNAL "temp variable for testing switches" FORCE)
    endif(force_testing${testName})
    if(compile_${testName})

      # compile the program
      add_executable(${testName} ${testName}.cxx )
      target_link_libraries(${testName} ${ARGN})

    endif(compile_${testName})

  # if BUILD_TESTING is off, force the test to turn on next time BUILD_TESTING is turned on
  else(BUILD_TESTING)
    set(compile_${testName} OFF CACHE BOOL "Compile ${testName}" FORCE)
  endif(BUILD_TESTING)

endfunction(add_test_executable)

#
# Define a function that adds a ctest test only if the executable is turned on
#
function(add_ctest_test testName)
  if(BUILD_TESTING AND compile_${testName})
    add_test(${testName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testName} ${ARGN})
  endif(BUILD_TESTING AND compile_${testName})
endfunction(add_ctest_test)

function(add_ctest_subtest testName subtestName)
  if(BUILD_TESTING AND compile_${testName})
    add_test(${testName}_${subtestName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testName} ${ARGN})
  endif(BUILD_TESTING AND compile_${testName})
endfunction(add_ctest_subtest)
#
# Define a function that adds a ctest test with image comparison only if the executable is turned on
#
function(add_ctest_test_testdriver_image_compare testName testDriver imBaseline imResult)
  if(BUILD_TESTING)
    add_test(${testName} ${testDriver}
      --compare ${imBaseline}
                ${imResult}
      ${testName}
      ${ARGN}
      )
  endif(BUILD_TESTING)
endfunction(add_ctest_test_testdriver_image_compare)

#
# Define a function with comparison option with subtests
#
function(add_ctest_subtest_testdriver_image_compare testName subtestName testDriver imBaseline imResult)
  if(BUILD_TESTING)
    add_test(${testName}_${subtestName} ${testDriver}
      --compare ${imBaseline}
                ${imResult}
      ${testName}
      ${ARGN}
      )
  endif(BUILD_TESTING)
endfunction(add_ctest_subtest_testdriver_image_compare)


#------------------------------------------------------------------------------
# New tests, added during the redevelopment of FRAT -> CALATK
#

# Set up source code files for all the tests which are processed by the testDriver

set(testDriverFiltering_SRCS
calatkGaussianBlurringTest.cxx
)

set(testDriverPDESolver_SRCS
calatkAdvectionTest.cxx
calatkInterpolationTest.cxx
)

set(testDriverAlgorithms_SRCS
calatkLDDMMTest.cxx
)

add_executable(testDriverFiltering testDriverFiltering.cxx ${testDriverFiltering_SRCS})
target_link_libraries(testDriverFiltering CALATK ${ITK_LIBRARIES})

add_ctest_test_testdriver_image_compare(calatkGaussianBlurringTest ${ALGORITHMS_TESTS_FILTERING}
    ${test_input_dir}/Baseline/I0-blurred-sig01-baseline.nrrd
    ${test_temp_dir}/I0_blurred.nrrd
    ${test_input_dir}/I0_short.nhdr ${test_temp_dir}/I0_blurred.nrrd
)

add_executable(testDriverPDESolver testDriverPDESolver.cxx ${testDriverPDESolver_SRCS})
target_link_libraries(testDriverPDESolver CALATK ${ITK_LIBRARIES})

add_ctest_subtest_testdriver_image_compare(calatkAdvectionTest xtranslation ${ALGORITHMS_TESTS_PDESOLVER}
    ${test_input_dir}/Baseline/I0-advected-baseline-x.nrrd
    ${test_temp_dir}/I0_advected.nrrd
    ${test_input_dir}/I0_short.nhdr ${test_temp_dir}/I0_advected.nrrd 1 0 0.25
)

add_ctest_subtest_testdriver_image_compare(calatkAdvectionTest ytranslation ${ALGORITHMS_TESTS_PDESOLVER}
    ${test_input_dir}/Baseline/I0-advected-baseline-y.nrrd
    ${test_temp_dir}/I0_advected.nrrd
    ${test_input_dir}/I0_short.nhdr ${test_temp_dir}/I0_advected.nrrd 0 1 0.25
)

add_ctest_subtest_testdriver_image_compare(calatkAdvectionTest xytranslation ${ALGORITHMS_TESTS_PDESOLVER}
    ${test_input_dir}/Baseline/I0-advected-baseline-diagonal.nrrd
    ${test_temp_dir}/I0_advected.nrrd
    ${test_input_dir}/I0_short.nhdr ${test_temp_dir}/I0_advected.nrrd 1 1 0.25
)

# Set up all the test which go through the standard (non-test-driver) testing mechanism

add_test_executable(calatkImageDataStructureTest CALATK ${ITK_LIBRARIES})
add_ctest_test(calatkImageDataStructureTest ${test_temp_dir})

add_test_executable(calatkJSONTest CALATK)
add_ctest_test(calatkJSONTest
  ${test_input_dir}/brainSlices/lddmm_ws_wt_slices.json
  ${test_temp_dir}/calatkJSONTest_lddmm_ws_wt_slices_output.json
  )

add_test_executable(calatkRegularizationSpacingTest CALATK)
add_ctest_test(calatkRegularizationSpacingTest
  ${test_input_dir}/brainSlices/ws_slice.nrrd
  ${test_input_dir}/brainSlices/wt_slice.nrrd
  0.9
  ${test_temp_dir}/calatkRegularizationSpacingTest_warped_source.nrrd
  ${test_temp_dir}/calatkRegularizationSpacingTest_target_map.mha
  )

# LDDMM tests
add_executable(testDriverAlgorithms testDriverAlgorithms.cxx ${testDriverAlgorithms_SRCS})
target_link_libraries(testDriverAlgorithms CALATK ${ITK_LIBRARIES})

# Add 1D Tests for different solvers
add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShooting1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          simplifiedShooting 1 float
			  ${test_input_dir}/1DCurveSource.nhdr
			  ${test_input_dir}/1DCurveTarget.nhdr
			  ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShooting1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          adjointShooting 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest relaxation1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          relaxation 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShooting1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          simplifiedShooting 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShooting1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          adjointShooting 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest relaxation1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          relaxation 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResult.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )


add_test_executable(GenerateBullseyes ${ITK_LIBRARIES})
if(BUILD_TESTING AND compile_GenerateBullseyes)
  add_test(GenerateBullseyes ${CMAKE_COMMAND}
    -DGenerateBullseyes_output_dir:PATH=${test_temp_dir}
    -DGenerateBullseyes_executable:FILEPATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GenerateBullseyes
    -P ${CALATK_SOURCE_DIR}/Code/Testing/GenerateBullseyesIfNeeded.cmake
    )
endif()
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
#add_test_executable(conservationTest CALATK ${ITK_LIBRARIES})

## FlowTest
#add_test_executable(FlowTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## vvImageTest
#add_test_executable(vvImageTest CALATK ${ITK_LIBRARIES})

## LDDMMTest
#add_test_executable(LDDMMTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## GradientTest
#add_test_executable(GradientTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## IOTest
#add_test_executable(IOTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesTest
#add_test_executable(TimeSeriesTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## LDDMMTestMultiscale
#add_test_executable(LDDMMTestMultiscale CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesTestMultiscale
#add_test_executable(TimeSeriesTestMultiscale CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## AtlasBuilderTest
#add_test_executable(AtlasBuilderTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## AtlasBuilderTestMultiscale
#add_test_executable(AtlasBuilderTestMultiscale CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## mapTest
#add_test_executable(mapTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## AffineITKTest
#add_test_executable(AffineITKTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesDataTest
#add_test_executable(TimeSeriesDataTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesComputePoints
#add_test_executable(TimeSeriesComputePoints CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## LongitudinalAtlasTest
#add_test_executable(LongitudinalAtlasTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## resizeImage
#add_test_executable(resizeImage CALATK ${ITK_LIBRARIES})

## imageMask
#add_test_executable(imageMask CALATK ${ITK_LIBRARIES})

## TimeSeriesFinalEnergy
#add_test_executable(TimeSeriesFinalEnergy CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesWarpImageToTime
#add_test_executable(TimeSeriesWarpImageToTime CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesDetJacobian
#add_test_executable(TimeSeriesDetJacobian CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## TimeSeriesComputeMaps
#add_test_executable(TimeSeriesComputeMaps CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## ComputeDetJacobian
#add_test_executable(ComputeDetJacobian CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## GeometricMetamorphosis
#add_test_executable(GeometricMetamorphosisTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})

## GeometricMetamorphosisFigures
#add_test_executable(GeometricMetamorphosisFiguresTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})


#------------------------------------------------------------------------------
# Add test executables with ctest tests
#

## advectionTest
#add_test_executable(advectionTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})
#add_ctest_test(advectionTest ${test_temp_dir})

## VectorImage2DTest
#add_test_executable(VectorImage2DTest CALATK ${ITK_LIBRARIES})
#add_ctest_test(VectorImage2DTest)

## VectorImage3DTest
#add_test_executable(VectorImage3DTest CALATK ${ITK_LIBRARIES})
#add_ctest_test(VectorImage3DTest)

## convertITKMetaDataTest
#add_test_executable(convertITKMetaDataTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})
#add_ctest_test(convertITKMetaDataTest
#  ${test_input_dir}/2Dsample.mha
#  ${test_input_dir}/3Dsample.mha
#  )

## writeFileITKTest
#add_test_executable(writeFileITKTest CALATK ${FFTW_LIB} ${ITK_LIBRARIES})
#add_ctest_test(writeFileITKTest
#  # 2D input
#  ${test_input_dir}/2Dsample.mha
#  # 3D input
#  ${test_input_dir}/3Dsample.mha
#  # output dir
#  ${test_temp_dir}
#  )

## dirExistsTest
#add_test_executable(dirExistsTest CALATK ${ITK_LIBRARIES})
#add_ctest_test(dirExistsTest
#  # A valid directory path
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  )

## makeDirIfNeededTest
#add_test_executable(makeDirIfNeededTest CALATK ${ITK_LIBRARIES})
#add_ctest_test(makeDirIfNeededTest
#  # Temporary output path
#  ${test_temp_dir}
#  )

## removeDoubleSlashesTest
#add_test_executable(removeDoubleSlashesTest CALATK ${ITK_LIBRARIES})
#add_ctest_test(removeDoubleSlashesTest)

#------------------------------------------------------------------------------
# Cppcheck static code analysis test.
option( CALATK_USE_CPPCHECK 
  "Create a test that does static code analysis with cppcheck." OFF )
if( CALATK_USE_CPPCHECK )
  find_package( Cppcheck REQUIRED )
  set( cppcheck_paths_to_check "${CALATK_SOURCE_DIR}/Code" )
  set( cppcheck_suppressions_file "${CALATK_SOURCE_DIR}/Scripts/Cppcheck/CALATKCppcheck.supp" )
  set( cppcheck_cmd_prefix "${CPPCHECK_EXECUTABLE}" -q --xml --suppressions "${cppcheck_suppressions_file}" )
  add_test( CppcheckStyle ${cppcheck_cmd_prefix} --enable=style ${cppcheck_paths_to_check} )
  add_test( CppcheckPerformance ${cppcheck_cmd_prefix} --enable=performance ${cppcheck_paths_to_check} )
  add_test( CppcheckPortability ${cppcheck_cmd_prefix} --enable=portability ${cppcheck_paths_to_check} )
  add_test( CppcheckInformation ${cppcheck_cmd_prefix} --enable=information ${cppcheck_paths_to_check} )
  add_test( CppcheckUnusedFunction ${cppcheck_cmd_prefix} --enable=unusedFunction ${cppcheck_paths_to_check} )
  set_tests_properties( CppcheckStyle CppcheckPerformance CppcheckPortability
    CppcheckInformation CppcheckUnusedFunction
    PROPERTIES FAIL_REGULAR_EXPRESSION "error file="
    )
endif()

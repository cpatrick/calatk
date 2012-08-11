
# Set bin and output dirs for tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(test_temp_dir ${CALATK_BINARY_DIR}/Testing/Temporary)
set(test_input_dir ${CALATK_SOURCE_DIR}/TestingData)
set(test_bin_dir ${CALATK_BINARY_DIR}/Code/Testing)

# for calatkRegressionTestJSON.h
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set(ALGORITHMS_TESTS_FILTERING ${test_bin_dir}/testDriverFiltering)
set(ALGORITHMS_TESTS_PDESOLVER ${test_bin_dir}/testDriverPDESolver)
set(ALGORITHMS_TESTS_ALGORITHMS ${test_bin_dir}/testDriverAlgorithms)

# For performing JSON regression tests.
add_executable( RegressionTestJSON RegressionTestJSON.cxx ${CMAKE_SOURCE_DIR}/Code/External/jsoncpp/jsoncpp.cpp )
set(regression_test_json ${test_bin_dir}/RegressionTestJSON)

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
# Define a function that adds a ctest test with image comparison
#
function(add_ctest_test_testdriver testName testDriver)
  if(BUILD_TESTING)
    add_test(NAME ${testName}
      WORKING_DIRECTORY ${test_temp_dir}
      COMMAND ${testDriver}
      ${testName}
      ${ARGN}
      )
  endif(BUILD_TESTING)
endfunction(add_ctest_test_testdriver)
#
# Define a function that adds a ctest test with image comparison
#
function(add_ctest_test_testdriver_image_compare testName testDriver imBaseline imResult)
  if(BUILD_TESTING)
    add_test(NAME ${testName}
      WORKING_DIRECTORY ${test_temp_dir}
      COMMAND ${testDriver}
      --compareIntensityTolerance 0.1
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
    add_test(NAME ${testName}_${subtestName}
      WORKING_DIRECTORY ${test_temp_dir}
      COMMAND ${testDriver}
      --compareIntensityTolerance 0.1
      --compare ${imBaseline}
                ${imResult}
      ${testName}
      ${ARGN}
      )
    # set environment so it can also find test data which is specified through JSON files
    set_property(TEST ${testName}_${subtestName} PROPERTY ENVIRONMENT "CALATK.dataPath=${CALATK_SOURCE_DIR}/TestingData")
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
  calatkMetamorphosisTest.cxx
  calatkAtlasBuilderTest.cxx
  )

set(testDriverInterfaces_SRCS
  calatkApplicationImageFileNamesGivenTest.cxx
  calatkApplicationDataConfigTest.cxx
  )

set(testDriverImageManager_SRCS
  calatkBasicDataConfigTest.cxx
  calatkAdvancedDataConfigTest.cxx
  )

set(testDriverUtilities_SRCS
  calatkVectorFieldUtilsTest.cxx
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

add_test_executable(calatkCStateFactoryTest CALATK)
add_ctest_test(calatkCStateFactoryTest)

# LDDMM tests
add_executable(testDriverAlgorithms testDriverAlgorithms.cxx ${testDriverAlgorithms_SRCS})
target_link_libraries(testDriverAlgorithms CALATK ${ITK_LIBRARIES})

# Add 1D Tests for different solvers
add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShooting1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingFloat.nhdr
                          simplifiedShooting 1 float
			  ${test_input_dir}/1DCurveSource.nhdr
			  ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShooting1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingFloat.nhdr
                          adjointShooting 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShootingInitialImage1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingInitialImageFloat.nhdr
                          simplifiedShootingInitialImage 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingInitialImageFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShootingInitialImage1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingInitialImageFloat.nhdr
                          adjointShootingInitialImage 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingInitialImageFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest relaxation1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMRelaxationFloat.nhdr
                          relaxation 1 float
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMRelaxationFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShooting1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingDouble.nhdr
                          simplifiedShooting 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShooting1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointDouble.nhdr
                          adjointShooting 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest simplifiedShootingInitialImage1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingInitialImageDouble.nhdr
                          simplifiedShootingInitialImage 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMSimplifiedShootingInitialImageDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest adjointShootingInitialImage1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingInitialImageDouble.nhdr
                          adjointShootingInitialImage 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMAdjointShootingInitialImageDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkLDDMMTest relaxation1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMRelaxationDouble.nhdr
                          relaxation 1 double
                          ${test_input_dir}/1DCurveSource.nhdr
                          ${test_input_dir}/1DCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultLDDMMRelaxationDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatk1DTest.json
                          )

# Add metamorphosis tests
# Add 1D Tests for different solvers
add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest simplifiedMetamorphosis1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedFloat.nhdr
                          MetamorphosisSimplified 1 float
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest simplifiedMetamorphosis1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedDouble.nhdr
                          MetamorphosisSimplified 1 double
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest adjointMetamorphosis1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointFloat.nhdr
                          MetamorphosisFullAdjoint 1 float
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest adjointMetamorphosis1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointDouble.nhdr
                          MetamorphosisFullAdjoint 1 double
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
			  )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest simplifiedMetamorphosisInitialImage1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedInitialImageFloat.nhdr
                          MetamorphosisSimplifiedInitialImage 1 float
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedInitialImageFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest simplifiedMetamorphosisInitialImage1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedInitialImageDouble.nhdr
                          MetamorphosisSimplifiedInitialImage 1 double
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultSimplifiedInitialImageDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest adjointMetamorphosisInitialImage1DFloat ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointInitialImageFloat.nhdr
                          MetamorphosisFullAdjointInitialImage 1 float
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointInitialImageFloat.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
                          )

add_ctest_subtest_testdriver_image_compare(calatkMetamorphosisTest adjointMetamorphosisInitialImage1DDouble ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/1DMetamorphosisCurveResultBaseline.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointInitialImageDouble.nhdr
                          MetamorphosisFullAdjointInitialImage 1 double
                          ${test_input_dir}/1DMetamorphosisCurveSource.nhdr
                          ${test_input_dir}/1DMetamorphosisCurveTarget.nhdr
                          ${test_temp_dir}/1DCurveResultAdjointInitialImageDouble.nhdr
                          ${CALATK_SOURCE_DIR}/Code/Testing/calatkMetamorphosis1DTest.json
                          )


add_ctest_subtest_testdriver_image_compare(calatkAtlasBuilderTest atlasBuilder_atlasTarget_fullGradient ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/baseline_atlasIsTargetResult_fullGradient.nrrd
                          ${test_temp_dir}/atlasIsTargetResult_fullGradient.nrrd
                          ${CALATK_SOURCE_DIR}/Code/Testing/atlas.json
			  ${CALATK_SOURCE_DIR}/Code/Testing/atlasDataAtlasIsTargetTwoDatasets.json
			  ${test_temp_dir}/atlasIsTargetResult_fullGradient.nrrd 0
			  )

add_ctest_subtest_testdriver_image_compare(calatkAtlasBuilderTest atlasBuilder_atlasTarget_subiterations ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/baseline_atlasIsTargetResult_subiterations.nrrd
                          ${test_temp_dir}/atlasIsTargetResult_subiterations.nrrd
                          ${CALATK_SOURCE_DIR}/Code/Testing/atlas.json
			  ${CALATK_SOURCE_DIR}/Code/Testing/atlasDataAtlasIsTargetTwoDatasets.json
			  ${test_temp_dir}/atlasIsTargetResult_subiterations.nrrd 1
			  )

add_ctest_subtest_testdriver_image_compare(calatkAtlasBuilderTest atlasBuilder_atlasSource_fullGradient ${ALGORITHMS_TESTS_ALGORITHMS}
                          ${test_input_dir}/Baseline/baseline_atlasIsSourceResult_fullGradient.nrrd
                          ${test_temp_dir}/atlasIsSourceResult_fullGradient.nrrd
                          ${CALATK_SOURCE_DIR}/Code/Testing/atlas.json
			  ${CALATK_SOURCE_DIR}/Code/Testing/atlasDataAtlasIsSourceTwoDatasets.json
			  ${test_temp_dir}/atlasIsSourceResult_fullGradient.nrrd 0
			  )


add_test_executable(calatkImageManagerTest CALATK ${ITK_LIBRARIES})
add_ctest_test(calatkImageManagerTest)

# Tests for the high level interface to the library and visualization/analysis
# interfaces.
add_executable(testDriverInterfaces testDriverInterfaces.cxx ${testDriverInterfaces_SRCS})
target_link_libraries(testDriverInterfaces CALATK ${ITK_LIBRARIES})

add_test_executable(GenerateBullseyes ${ITK_LIBRARIES})
if(BUILD_TESTING AND compile_GenerateBullseyes)
  add_test(GenerateBullseyes ${CMAKE_COMMAND}
    -DGenerateBullseyes_output_dir:PATH=${test_temp_dir}
    -DGenerateBullseyes_executable:FILEPATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GenerateBullseyes
    -P ${CALATK_SOURCE_DIR}/Code/Testing/GenerateBullseyesIfNeeded.cmake
    )

  get_target_property(CALATKCommandLine_dir CALATKCommandLine
    RUNTIME_OUTPUT_DIRECTORY)
  set(CALATKCommandLine_path ${CALATKCommandLine_dir}/CALATKCommandLine)

  add_test(CALATKCommandLineNoArgs ${CALATKCommandLine_path})
  add_test(CALATKCommandLineHelp ${CALATKCommandLine_path} -h)
  add_test(CALATKCommandInsufficientArgs ${CALATKCommandLine_path}
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfig.json
    ${test_temp_dir}/Bullseye1DTimePoint0.mha
    )
  set_tests_properties(CALATKCommandLineNoArgs
    CALATKCommandLineHelp
    CALATKCommandInsufficientArgs
    PROPERTIES WILL_FAIL TRUE
    )

  add_test(NAME CALATKCommandLineBullseyes1DTest
    WORKING_DIRECTORY ${test_temp_dir}
    COMMAND ${CALATKCommandLine_path}
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfig.json
    Bullseye1DTimePoint0.mha
    Bullseye1DTimePoint1.mha
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_AlgorithmConfig.json
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_DataConfig.json
    )
  add_test(CALATKCommandLineBullseyes1DTest_CleanedAlgorithmConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_AlgorithmConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLineBullseyes1D_AlgorithmConfigBaseline.json
    )
  add_test(CALATKCommandLineBullseyes1DTest_CleanedDataConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_DataConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLineBullseyes1D_DataConfigBaseline.json
    )
  set_tests_properties(CALATKCommandLineBullseyes1DTest_CleanedAlgorithmConfigVerification
    CALATKCommandLineBullseyes1DTest_CleanedDataConfigVerification
    PROPERTIES DEPENDS CALATKCommandLineBullseyes1DTest
    )

  add_test(NAME CALATKCommandLineBullseyes1DOutputTest
    WORKING_DIRECTORY ${test_temp_dir}
    COMMAND ${CALATKCommandLine_path}
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfig.json
    Bullseye1DTimePoint0.mha
    Bullseye1DTimePoint1.mha
    Bullseye1DTimePoint0WarpedTo1.mha
    ${test_temp_dir}/CALATKCommandLineBullseyes1DOutput_AlgorithmConfig.json
    ${test_temp_dir}/CALATKCommandLineBullseyes1DOutput_DataConfig.json
    )
  add_test(CALATKCommandLineBullseyes1DOutputTest_CleanedAlgorithmConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/CALATKCommandLineBullseyes1DOutput_AlgorithmConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLineBullseyes1DOutput_AlgorithmConfigBaseline.json
    )
  add_test(CALATKCommandLineBullseyes1DOutputTest_CleanedDataConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/CALATKCommandLineBullseyes1DOutput_DataConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/CALATKCommandLineBullseyes1DOutput_DataConfigBaseline.json
    )
  set_tests_properties(CALATKCommandLineBullseyes1DOutputTest_CleanedAlgorithmConfigVerification
    CALATKCommandLineBullseyes1DOutputTest_CleanedDataConfigVerification
    PROPERTIES DEPENDS CALATKCommandLineBullseyes1DOutputTest
    )

  add_test(CALATKCommandLineBullseyes1D_ImageDimensionsSpecifiedTest
    ${CALATKCommandLine_path}
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfigImageDimensionsSpecified.json
    ${test_temp_dir}/Bullseye1DTimePoint0.mha
    ${test_temp_dir}/Bullseye1DTimePoint1.mha
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_ImageDimensionSpecified_AlgorithmConfig.json
    ${test_temp_dir}/CALATKCommandLineBullseyes1D_ImageDimensionSpecified_DataConfig.json
    )

  # For profiling performance,
  #
  #  Bullseye1DTimePoint0.mha
  #  Bullseye1DTimePoint1.mha
  #
  # can be changed to
  #
  #  Bullseye2DTimePoint0.mha
  #  Bullseye2DTimePoint1.mha
  add_ctest_test_testdriver(calatkApplicationImageFileNamesGivenTest
    testDriverInterfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfig.json
    Bullseye1DTimePoint0.mha
    Bullseye1DTimePoint1.mha
    calatkApplicationImageFileNamesGivenTest_AlgorithmConfig.json
    calatkApplicationImageFileNamesGivenTest_DataConfig.json
    )
  set_tests_properties( calatkApplicationImageFileNamesGivenTest
    PROPERTIES WORKING_DIRECTORY ${test_temp_dir} )
  add_test(calatkApplicationImageFileNamesGivenTest_CleanedAlgorithmConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/calatkApplicationImageFileNamesGivenTest_AlgorithmConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationImageFileNamesGivenTest_CleanedAlgorithmConfigBaseline.json
    )
  add_test(calatkApplicationImageFileNamesGivenTest_CleanedDataConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/calatkApplicationImageFileNamesGivenTest_DataConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationImageFileNamesGivenTest_CleanedDataConfigBaseline.json
    )
  set_tests_properties(calatkApplicationImageFileNamesGivenTest_CleanedAlgorithmConfigVerification
    calatkApplicationImageFileNamesGivenTest_CleanedDataConfigVerification
    PROPERTIES DEPENDS calatkApplicationImageFileNamesGivenTest
    )

  add_ctest_test_testdriver(calatkApplicationDataConfigTest
    testDriverInterfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestAlgorithmConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationTestDataConfig.json
    ${test_temp_dir}/calatkApplicationDataConfigTest_AlgorithmConfig.json
    ${test_temp_dir}/calatkApplicationDataConfigTest_DataConfig.json
    )
  add_test(calatkApplicationDataConfigTest_CleanedAlgorithmConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/calatkApplicationDataConfigTest_AlgorithmConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationDataConfigTest_CleanedAlgorithmConfigBaseline.json
    )
  add_test(calatkApplicationDataConfigTest_CleanedDataConfigVerification
    ${regression_test_json}
    ${test_temp_dir}/calatkApplicationDataConfigTest_DataConfig.json
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkApplicationDataConfigTest_CleanedDataConfigBaseline.json
    )
  set_tests_properties(calatkApplicationDataConfigTest_CleanedAlgorithmConfigVerification
    calatkApplicationDataConfigTest_CleanedDataConfigVerification
    PROPERTIES DEPENDS calatkApplicationDataConfigTest
    )

  set_tests_properties(calatkApplicationImageFileNamesGivenTest
    CALATKCommandLineBullseyes1DTest
    CALATKCommandLineBullseyes1D_ImageDimensionsSpecifiedTest
    calatkApplicationDataConfigTest
    PROPERTIES DEPENDS GenerateBullseyes
    )
endif()

if( BUILD_TESTING )
  add_executable(testDriverImageManager testDriverImageManager.cxx ${testDriverImageManager_SRCS})
  target_link_libraries(testDriverImageManager CALATK ${ITK_LIBRARIES})

  add_test(calatkBasicDataConfigTest
    testDriverImageManager
    --compare-json ${CMAKE_CURRENT_SOURCE_DIR}/calatkBasicDataConfigTestOutputBaseline.json
                   ${test_temp_dir}/calatkBasicDataConfigTestOutput.json
    calatkBasicDataConfigTest
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkBasicDataConfigTestInput.json
    ${test_temp_dir}/calatkBasicDataConfigTestOutput.json
    )

  add_test(calatkAdvancedDataConfigTest
    testDriverImageManager
    --compare-json ${CMAKE_CURRENT_SOURCE_DIR}/calatkAdvancedDataConfigTestOutputBaseline.json
                   ${test_temp_dir}/calatkAdvancedDataConfigTestOutput.json
    calatkAdvancedDataConfigTest
    ${CMAKE_CURRENT_SOURCE_DIR}/calatkAdvancedDataConfigTestInput.json
    ${test_temp_dir}/calatkAdvancedDataConfigTestOutput.json
    )

  # LDDMM tests
  add_executable(testDriverUtilities testDriverUtilities.cxx ${testDriverUtilities_SRCS})
  target_link_libraries(testDriverUtilities CALATK ${ITK_LIBRARIES})

  add_test( calatkVectorFieldUtilsTest
    testDriverUtilities
    --compare
      ${test_input_dir}/Baseline/VectorFieldUtilsTest2DDeformationFieldBaseline.mha
      ${test_temp_dir}/VectorFieldUtilsTest2DDeformationField.mha
    calatkVectorFieldUtilsTest
    ${test_temp_dir}/VectorFieldUtilsTest2DDeformationField.mha
    ${test_temp_dir}/VectorFieldUtilsTest3DDeformationField.mha
    )
endif( BUILD_TESTING )

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
  set( cppcheck_paths_to_check
    "${CALATK_SOURCE_DIR}/Code/Applications"
    "${CALATK_SOURCE_DIR}/Code/Libraries"
    "${CALATK_SOURCE_DIR}/Code/Testing"
    )
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

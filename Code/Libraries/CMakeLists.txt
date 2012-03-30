set(CALATK_jsoncpp_source ${CMAKE_SOURCE_DIR}/Code/External/jsoncpp/jsoncpp.cpp)

# Create a single library for the toolkit
# Set up sources for Algorithms
set(CALATK_Algorithms_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/CAlgorithmBase.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/CAlgorithmFactory.cxx
  )
# Set up sources for Base
set(CALATK_Base_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Base/CBase.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Base/CProcessBase.cxx
  )
# Set up sources for DataStructures
set(CALATK_DataStructures_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStructures/CALATKCommon.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStructures/VectorArray.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStructures/VectorField.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStructures/VectorImage.cxx
  )
# Set up sources for Interpolators
set(CALATK_Interpolators_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Interpolators/CCubicConvolutionInterpolator.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Interpolators/CFastLinearInterpolator.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Interpolators/CLinearInterpolator.cxx
  )
# Set up sources for Kernels
set(CALATK_Kernels_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Kernels/CFourierDomainKernel.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Kernels/CGaussianKernel.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Kernels/CHelmholtzKernel.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Kernels/CMultiGaussianKernel.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Kernels/CKernelFactory.cxx
  )
# Set up sources for Metrics
set(CALATK_Metrics_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Metrics/CMetricSSD.cxx
  )
# Set up sources for Numerics
set(CALATK_Numerics_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Numerics/COneStepEvolverSemiLagrangianAdvection.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Numerics/CStationaryEvolver.cxx
  )
# Set up sources for ObjectiveFunctions
set(CALATK_ObjectiveFunctions_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/ObjectiveFunctions/CObjectiveFunctionFactory.cxx
  )
# Set up sources for Interfaces
set(CALATK_Interfaces_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/CApplication.cxx
  )
# Set up sources for States
set(CALATK_States_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/States/CStateInitialImageMomentum.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/States/CStateSpatioTemporalVelocityField.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/States/CStateFactory.cxx
  )
# Set up sources for Utilities
set(CALATK_Utilities_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Timer.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/ApplicationUtils.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/CJSONConfiguration.cxx
  )
set(CALATK_sources
  ${CALATK_Algorithms_sources}
  ${CALATK_Base_sources}
  ${CALATK_DataStructures_sources}
  ${CALATK_Interpolators_sources}
  ${CALATK_Kernels_sources}
  ${CALATK_Metrics_sources}
  ${CALATK_Numerics_sources}
  ${CALATK_ObjectiveFunctions_sources}
  ${CALATK_Interfaces_sources}
  ${CALATK_States_sources}
  ${CALATK_Utilities_sources}
  ${CALATK_jsoncpp_source}
  )

if(USE_LBFGS)
  # Set up source for LBFGS
  set(CALATK_libbfgs_source ${CMAKE_SOURCE_DIR}/Code/External/libbfgs/lbfgs.c)
  set(CALATK_sources
    ${CALATK_sources}
    ${CALATK_libbfgs_source}
    )
endif(USE_LBFGS)

add_library(CALATK ${CALATK_sources})
target_link_libraries(CALATK ${FFTW_LIB} ${FFTWF_LIB} ${ITK_LIBRARIES} ${NLOPT_LIB} ${IPOPT_LIBS})

install( TARGETS CALATK
	 RUNTIME DESTINATION bin
	 LIBRARY DESTINATION lib
	 ARCHIVE DESTINATION lib/static)

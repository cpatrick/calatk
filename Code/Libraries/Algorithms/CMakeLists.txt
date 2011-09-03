
# Set up source for Algorithms
set(FRAT_Algorithms_source
  ${CMAKE_CURRENT_SOURCE_DIR}/FlowAdvectionSemiLagrangian.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/FlowScalarConservation.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilder.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/AtlasBuilderMultiscale.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/GeometricMetamorphosis.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/GeometricMetamorphosisMultiscale.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/LDDMMAdjoint.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/TimeSeries.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/TimeSeriesMultiscale.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/LDDMMAdjointMultiscale.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/LongitudinalAtlasBuilder.cxx
  CACHE INTERNAL ""
)

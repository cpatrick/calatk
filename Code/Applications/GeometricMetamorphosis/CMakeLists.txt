#
# Add the Geometric Metamorphosis application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(GeometricMetamorphosis GeometricMetamorphosis ${libraries})

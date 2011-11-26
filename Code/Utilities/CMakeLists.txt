#
# Add all sorts of utility functions
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(applyMap applyMap ${libraries})

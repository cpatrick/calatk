#
# Add all sorts of utility functions
#
SET(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
add_clp_application(applyMap applyMap ${libraries})

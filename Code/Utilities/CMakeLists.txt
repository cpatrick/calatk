#
# Add all sorts of utility functions
#
if(COMPILE_CLP_MODULES)
  set(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
  add_clp_application(applyMap applyMap ${libraries})
endif()

#
# Add the Atlas Builder application
#
SET(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
add_clp_application(AtlasBuilder AtlasBuilder ${libraries})

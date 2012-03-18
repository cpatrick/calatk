#
# Add the LDDM shooting application
#
SET(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
add_clp_application(LDDMMShooting LDDMMShooting ${libraries})

#
# Add the LDDM application
#
SET(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
add_clp_application(LDDMMRelaxation LDDMMRelaxation ${libraries})

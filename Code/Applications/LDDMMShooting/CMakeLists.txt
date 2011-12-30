#
# Add the LDDM shooting application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(LDDMMShooting LDDMMShooting ${libraries})

#
# Add the LDDM application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(LDDMMRelaxation LDDMMRelaxation ${libraries})

#
# Add the LDDM application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(testme testme ${FFTW_LIB} ${libraries})

#
# Add the Metamorphosis shooting application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(MetamorphosisShooting MetamorphosisShooting ${libraries})

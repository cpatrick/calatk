#
# Add the Atlas Builder application
#
SET(libraries ${FFTW_LIB} ITKCommon ITKIO itksys)
add_clp_application(AtlasBuilder AtlasBuilder ${libraries})

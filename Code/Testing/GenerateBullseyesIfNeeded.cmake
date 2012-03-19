# On generate the bullseyes testing data input in needed.
if( NOT EXISTS ${GenerateBullseyes_output_dir}/Bullseye3DTimePoint0.mha )
  message( "COMMAND ${GenerateBullseyes_executable} ${GenerateBullseyes_output_dir}" )
  execute_process( COMMAND ${GenerateBullseyes_executable} ${GenerateBullseyes_output_dir}/Bullseye )
endif()

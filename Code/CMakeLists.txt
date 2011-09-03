
# First go to Libraries, then Applications
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Applications)

# Set up testing
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Testing)

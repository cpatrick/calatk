set(CTEST_PROJECT_NAME "CALATK")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "www.cdash.org")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=calaTK")
set(CTEST_DROP_SITE_CDASH TRUE)

message( "${CMAKE_CURRENT_SOURCE_DIR} hi mom" )
set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE
  "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Valgrind/CALATKValgrind.supp"
  )


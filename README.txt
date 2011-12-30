Installation:

1) mkdir calatk-build
2) cd calatk-build
3) ccmake [directory_containing_calatk_source]
4) make
5) make doc

Making a binary installer
6) cpack -C CPackConfig.cmake

Making a source installer
7) cpack -C CPackSourceConfig.cmake



SET(pwd ${CMAKE_CURRENT_LIST_DIR})

SET(src "")
AUX_SOURCE_DIRECTORY(${pwd} src)
add_executable(minecraft ${src})
message(STATUS "minecraft added ${src}")

target_link_libraries(minecraft ${stdgl_libraries})
FIND_PACKAGE(JPEG REQUIRED)
TARGET_LINK_LIBRARIES(minecraft ${JPEG_LIBRARIES})
TARGET_LINK_LIBRARIES(minecraft pmdreader)

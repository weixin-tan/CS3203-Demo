file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
add_library(qps ${srcs} ${headers})
target_include_directories(qps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


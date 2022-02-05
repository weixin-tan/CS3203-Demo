file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/SuchThatHandler.cpp src/SuchThatHandler.h src/QPSMainLogic.cpp src/QPSMainLogic.h src/Result.cpp src/Result.h src/ResultProcessor.cpp src/ResultProcessor.h src/ResultFormatter.cpp src/ResultFormatter.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/PKB")
target_link_libraries(spa pkb)

cmake_minimum_required(VERSION 3.28)
project(bubbles LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)



set(SOURCES
    src/main.cpp
    src/bubble.cpp
    src/menu.cpp
    src/window.cpp
)

set(HEADERS
    src/bubble.hpp
    src/menu.hpp
    src/window.hpp
)

add_executable(run ${SOURCES} ${HEADERS})
target_compile_features(run PRIVATE cxx_std_17)
target_link_libraries(run PRIVATE SFML::Graphics)
target_link_libraries(run PRIVATE SFML::Audio)

add_custom_target(resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:run>/resources
)
add_dependencies(run resources)


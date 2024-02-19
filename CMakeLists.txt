cmake_minimum_required(VERSION 3.16)
project(RockPaperScissors LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-std=c++20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(RockPaperScissors 
    src/main.cpp
    src/Object.cpp
    src/TextureManager.cpp
)

target_include_directories(RockPaperScissors PRIVATE include)
target_compile_features(RockPaperScissors PRIVATE cxx_std_20)
target_link_libraries(RockPaperScissors PRIVATE sfml-graphics)

if(WIN32)
    add_custom_command(
        TARGET RockPaperScissors
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:RockPaperScissors>
        VERBATIM)
endif()

install(TARGETS RockPaperScissors)

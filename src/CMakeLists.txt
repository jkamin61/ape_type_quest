cmake_minimum_required(VERSION 3.16)
project(ApeQuestType LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)

add_executable(ape_quest_type
        main.cpp
        Word.cpp
        Game.cpp
        Settings.cpp
        ../include/Game.h
        ../include/Settings.h
)

target_link_libraries(ape_quest_type PRIVATE fmt SFML::Graphics SFML::Window SFML::System)

file(COPY ${CMAKE_SOURCE_DIR}/../assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
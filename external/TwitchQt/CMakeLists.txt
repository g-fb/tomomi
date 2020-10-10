cmake_minimum_required(VERSION 3.14)

project(TwitchQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TwitchQt INTERFACE)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchapi.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchbadge.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchbadgereply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchemote.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchemotereply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchgame.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchgamereply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchreply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchstream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchstreamreply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchuser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/twitchuserreply.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json/json.hpp
)
target_sources(TwitchQt INTERFACE "$<BUILD_INTERFACE:${header_files}>")

target_include_directories(TwitchQt INTERFACE src/)

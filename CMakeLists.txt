cmake_minimum_required(VERSION 3.22)
project(GameOOP LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-3.1.0/lib/cmake/SFML" CACHE PATH "Path to SFML cmake config")

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(GameOOP ${GAME_SOURCES})

target_include_directories(GameOOP PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GameOOP PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# Copy assets folder next to the .exe after build so sf::Texture::loadFromFile
# / openFromFile can find them with relative paths like "assets/images/..."
add_custom_command(TARGET GameOOP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:GameOOP>/assets"
)
add_custom_command(TARGET GameOOP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:GameOOP>/saves"
)

# On Windows, copy the SFML .dll files next to the .exe so the game runs
# without manually adding SFML's bin/ folder to PATH.
if (WIN32)
    add_custom_command(TARGET GameOOP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:GameOOP>
            $<TARGET_FILE_DIR:GameOOP>
        COMMAND_EXPAND_LISTS
    )
endif()

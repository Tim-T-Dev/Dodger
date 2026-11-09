cmake_minimum_required(VERSION 3.16)
project(Dodger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML Library
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system network audio REQUIRED)

# Source Files
set(SOURCES
    src/main.cpp
    src/game.hpp
    src/game.cpp

    src/entities/player.hpp
    src/entities/player.cpp
    src/entities/enemy.hpp
    src/entities/enemy.cpp
    src/entities/particle.hpp
    src/entities/particle.cpp
    src/entities/projectile.hpp
    src/entities/projectile.cpp
    src/entities/character.cpp
    src/entities/character.hpp

    src/functionality/animator.hpp
    src/functionality/animator.cpp
    src/functionality/randomNumber.hpp
    src/functionality/randomNumber.cpp
    src/functionality/hitbox.hpp
    src/functionality/hitbox.cpp
    src/functionality/direction.hpp
    src/functionality/gameState.hpp
    src/functionality/pausableClock.hpp
    src/functionality/pausableClock.cpp
)

add_executable(Dodger ${SOURCES})

# Include Paths
target_include_directories(Dodger PRIVATE
    src
    src/entities
    src/functionality
)

# Link SFML
target_link_libraries(Dodger PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-network 
    sfml-audio
)

# --- POST BUILD ---

# Copy SFML DLLs
file(GLOB SFML_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/resources/SFML-2.6.1/bin/*.dll")
foreach(DLL ${SFML_DLLS})
    add_custom_command(TARGET Dodger POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}"
            "$<TARGET_FILE_DIR:Dodger>"
        COMMENT "Kopiere ${DLL}..."
    )
endforeach()

# Copy resource folder
add_custom_command(TARGET Dodger POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Dodger>/resources"
    COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:Dodger>/resources/SFML-2.6.1"
    COMMENT "Synchronisiere Ressourcen..."
)
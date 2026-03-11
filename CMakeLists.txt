cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0167 NEW)
project(GameEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Find packages ──────────────────────────────────────────────────────────────
find_package(SDL2 REQUIRED)

# ZeroMQ
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(ZMQ REQUIRED libzmq)
else()
    # Fallback for Windows — expects ZMQ in a standard location
    find_path(ZMQ_INCLUDE_DIRS zmq.hpp)
    find_library(ZMQ_LIBRARIES NAMES zmq libzmq)
    if(NOT ZMQ_INCLUDE_DIRS OR NOT ZMQ_LIBRARIES)
        message(FATAL_ERROR "ZeroMQ not found. On Windows, install via vcpkg: vcpkg install zeromq cppzmq")
    endif()
endif()
# After the existing find_package lines
find_package(Boost REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# ── Source files ───────────────────────────────────────────────────────────────
set(ENGINE_SOURCES
    src/core/entity.cpp
    src/core/eventmanager.cpp
    src/core/init.cpp
    src/core/timeline.cpp
    src/core/game.cpp
    src/handlers/collision_handler.cpp
    src/handlers/control_handler.cpp
    src/handlers/death_handler.cpp
    src/handlers/sideboundary_handler.cpp
    src/modules/collisions.cpp
    src/modules/control.cpp
    src/modules/deathzone.cpp
    src/modules/gravity.cpp
    src/modules/movement.cpp
    src/modules/platform.cpp
    src/modules/sideboundary.cpp
    src/modules/spawnpoint.cpp
    src/networking/peer.cpp
    src/rendering/render.cpp
    src/rendering/scaling.cpp
)

# ── Executable ─────────────────────────────────────────────────────────────────
add_executable(main main.cpp ${ENGINE_SOURCES})

# ── Include paths ──────────────────────────────────────────────────────────────
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS} 
)

# ── Link libraries ─────────────────────────────────────────────────────────────
target_link_libraries(main PRIVATE
    ${SDL2_LIBRARIES}
    ${ZMQ_LIBRARIES}
)

# ── Compiler flags ─────────────────────────────────────────────────────────────
target_compile_options(main PRIVATE -Wall -g)
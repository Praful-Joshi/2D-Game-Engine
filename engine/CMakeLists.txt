add_library(nova2d_engine STATIC
    core/Logger.cpp
    core/Clock.cpp
    core/Window.cpp
    core/Engine.cpp
)

target_include_directories(nova2d_engine
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}"   # so #include "core/Logger.hpp" works
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(nova2d_engine
    PUBLIC  SDL2::SDL2
    PRIVATE spdlog::spdlog
)

set_project_warnings(nova2d_engine)
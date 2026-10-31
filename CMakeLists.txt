cmake_minimum_required(VERSION 3.16)
project(qbe-apps C)

# ---------------------------------------------------------------------------
# SDL2 via pkg-config. This returns the correct flags on every platform
# (Windows/mingw: -Dmain=SDL_main -mwindows -lSDL2main -lSDL2; Linux: -lSDL2),
# unlike a bare `-lSDL2 -lm` which is wrong on Windows.
# ---------------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

# ---------------------------------------------------------------------------
# Auto-discover every *.c program in c-lang/. Drop a new demo into c-lang/
# and it is picked up automatically on the next configure.
# ---------------------------------------------------------------------------
file(GLOB C_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/c-lang/*.c")

foreach(src IN LISTS C_SOURCES)
    get_filename_component(name "${src}" NAME_WE)   # bounce.c -> bounce

    add_executable(${name} "${src}")
    target_link_libraries(${name} PRIVATE PkgConfig::SDL2)

    # libm is separate on Linux/macOS; on Windows the math funcs live in the CRT.
    if(UNIX)
        target_link_libraries(${name} PRIVATE m)
    endif()

    # run-<name>: build then run this one program.
    add_custom_target(run-${name}
        COMMAND $<TARGET_FILE:${name}>
        DEPENDS ${name}
        COMMENT "Running ${name}"
        VERBATIM)

    list(APPEND RUN_TARGETS run-${name})
endforeach()

# Convenience: run every demo in sequence (each blocks until its window closes).
if(RUN_TARGETS)
    add_custom_target(run-all DEPENDS ${RUN_TARGETS})
endif()

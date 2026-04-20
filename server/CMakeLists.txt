project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main.cpp)

if(WIN32)
    target_link_libraries(server PRIVATE Boost::boost ws2_32)
else()
    target_link_libraries(server PRIVATE Boost::boost)
endif()

target_compile_definitions(server PRIVATE
    GC_LIB_DIR="${GC_LIB_DIR}"
    GC_LIB_SUFFIX="${GC_LIB_SUFFIX}"
    GC_LIB_EXTENSION="${CMAKE_SHARED_LIBRARY_SUFFIX}")

target_include_directories(server PRIVATE include)
target_precompile_headers(server PRIVATE include/stdafx.h)

if (MSVC)
    target_compile_options(server PRIVATE /W4 /wd4244)
    target_compile_definitions(server PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(server PRIVATE -Wall -Wextra)
endif()

target_compile_definitions(server PRIVATE WIN32_LEAN_AND_MEAN)

file(GLOB PROTOBUFS ../protobufs/*.cc)
target_include_directories(server PRIVATE ../protobufs)
target_sources(server PRIVATE ${PROTOBUFS})
source_group("Protobufs" FILES ${PROTOBUFS})

if (OUTDIR)
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:server>
        ${OUTDIR}/$<TARGET_FILE_NAME:server>)
endif()
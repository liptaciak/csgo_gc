project(client)

add_library(csgo_gc SHARED
        src/appid.cpp
        src/case_opening.cpp
        src/config.cpp
        src/gc_client.cpp
        src/gc_message.cpp
        src/gc_server.cpp
        src/gc_shared.cpp
        src/graffiti.cpp
        src/inventory.cpp
        src/item_schema.cpp
        src/keyvalue.cpp
        src/main.cpp
        src/networking_client.cpp
        src/networking_server.cpp
        src/steam_hook.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(csgo_gc PRIVATE src/platform_windows.cpp)
else()
    target_sources(csgo_gc PRIVATE src/platform_unix.cpp)
endif()

target_include_directories(csgo_gc PRIVATE ../deps/asio/include)

target_include_directories(csgo_gc PRIVATE include)
target_precompile_headers(csgo_gc PRIVATE include/stdafx.h)

set_target_properties(csgo_gc PROPERTIES PREFIX "")

if (MSVC)
    target_compile_options(csgo_gc PRIVATE /W4 /wd4244)
    target_compile_definitions(csgo_gc PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(csgo_gc PRIVATE -Wall -Wextra)
endif()

file(GLOB PROTOBUFS ../protobufs/*.cc)
target_include_directories(csgo_gc PRIVATE ../protobufs)
target_sources(csgo_gc PRIVATE ${PROTOBUFS})
source_group("Protobufs" FILES ${PROTOBUFS})

target_link_libraries(csgo_gc PRIVATE
    cryptopp
    funchook-static
    libprotobuf-lite
    steam_api)

# apple message box
if (APPLE)
    target_link_libraries(csgo_gc PRIVATE "-framework Foundation")
endif()

if (OUTDIR)
    add_custom_command(TARGET csgo_gc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:csgo_gc>
        ${OUTDIR}/csgo_gc/${GC_LIB_DIR}/$<TARGET_FILE_NAME:csgo_gc>)
endif()

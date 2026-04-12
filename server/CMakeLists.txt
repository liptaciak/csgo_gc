project(server)

add_executable(server main.cpp)

if(WIN32)
    target_link_libraries(server PRIVATE cppserver ws2_32)
else()
    target_link_libraries(server PRIVATE cppserver)
endif()

target_compile_definitions(server PRIVATE
    GC_LIB_DIR="${GC_LIB_DIR}"
    GC_LIB_SUFFIX="${GC_LIB_SUFFIX}"
    GC_LIB_EXTENSION="${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

if (OUTDIR)
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:server>
        ${OUTDIR}/$<TARGET_FILE_NAME:server>
    )
endif()
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server main.cpp)

target_compile_definitions(server PRIVATE
    BOOST_ASIO_HAS_CO_AWAIT
    _WIN32_WINNT=0x0A00
)

if(WIN32)
    target_link_libraries(server PRIVATE Boost::boost ws2_32)
else()
    target_link_libraries(server PRIVATE Boost::boost)
endif()

target_compile_definitions(server PRIVATE
    GC_LIB_DIR="${GC_LIB_DIR}"
    GC_LIB_SUFFIX="${GC_LIB_SUFFIX}"
    GC_LIB_EXTENSION="${CMAKE_SHARED_LIBRARY_SUFFIX}")

if (OUTDIR)
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:server>
        ${OUTDIR}/$<TARGET_FILE_NAME:server>)
endif()
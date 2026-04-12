cmake_minimum_required(VERSION 3.20)

project(csgo_gc)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(IS_32BIT ON)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if (IS_32BIT)
        set(GC_LIB_DIR ".")
        set(GC_EXE_SUFFIX "_osx")
    else()
        set(GC_LIB_DIR "osx64")
        set(GC_EXE_SUFFIX "_osx64")
    endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (IS_32BIT)
        set(GC_LIB_DIR ".")
        set(GC_EXE_SUFFIX "_linux")
    else()
        set(GC_LIB_DIR "linux64")
        set(GC_LIB_SUFFIX "_client")
        set(GC_EXE_SUFFIX "_linux64")
    endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (IS_32BIT)
        set(GC_LIB_DIR ".")
    else()
        set(GC_LIB_DIR "x64")
        set(GC_EXE_SUFFIX "_win64")
    endif()
else()
    message(FATAL_ERROR "Unsupported system ${CMAKE_SYSTEM_NAME}")
endif()

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    add_compile_options(-fpic -fvisibility=hidden)

    if (NOT APPLE)
        add_link_options(-Wl,--no-undefined -z noexecstack)
    endif()
endif()

add_subdirectory(csgo_gc)
add_subdirectory(server)
add_subdirectory(launcher)
add_subdirectory(steamworks)

set(CPPSERVER_USE_SSL ON CACHE BOOL "")
add_subdirectory(cppserver)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

#------------------------------------------------- 
# protobuf 
#-------------------------------------------------
option(protobuf_INSTALL OFF)
option(protobuf_BUILD_TESTS OFF)
option(protobuf_BUILD_PROTOC_BINARIES OFF)
option(protobuf_BUILD_SHARED_LIBS OFF)
option(protobuf_WITH_ZLIB OFF)

FetchContent_Declare(protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf
    GIT_TAG 2798a968c330de223b711e4fe504800280f333fb
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(protobuf)

#------------------------------------------------- 
# cryptopp
#-------------------------------------------------
option(CRYPTOPP_BUILD_TESTING OFF)
option(CRYPTOPP_INSTALL OFF)

FetchContent_Declare(cryptopp-cmake
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(cryptopp-cmake)

if (MSVC)
    target_compile_definitions(cryptopp PRIVATE _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
endif()

#------------------------------------------------- 
# funchook 
#-------------------------------------------------
option(FUNCHOOK_BUILD_SHARED OFF)
option(FUNCHOOK_BUILD_TESTS OFF)
option(FUNCHOOK_INSTALL OFF)

FetchContent_Declare(funchook
    GIT_REPOSITORY https://github.com/kubo/funchook
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(funchook)
cmake_minimum_required(VERSION 3.23)
project(portscan C)

set(CMAKE_C_STANDARD 17)

add_executable(portscan main.c common.h)

if (WIN32)
    target_link_libraries(portscan Ws2_32)
endif ()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")

if (WIN32)
    add_custom_command(TARGET portscan POST_BUILD
            COMMAND echo F | xcopy /y $<1:"${CMAKE_CURRENT_BINARY_DIR}\\portscan.exe"> $<1:"d:/jgy/bin\\portscan.exe">
            )
endif ()
cmake_minimum_required(VERSION 3.7.2)
project(cvicenie7 C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(tcp_server tcp_server.c)
add_executable(tcp_client tcp_client.c)
target_link_libraries(tcp_server ${CURSES_LIBRARIES})
target_link_libraries(tcp_client ${CURSES_LIBRARIES})
target_link_libraries(tcp_server pthread)
target_link_libraries(tcp_client pthread)

cmake_minimum_required(VERSION 2.4.0)
project (rctest)

include_directories(../include)

file(GLOB SOURCES "*.c")
file(GLOB LIBRARIES "../*.dylib")
message("LIBRARIES = ${LIBRARIES}")

add_executable(rctest ${SOURCES})
target_link_libraries(rctest ${LIBRARIES})
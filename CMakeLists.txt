cmake_minimum_required(VERSION 3.10)
project(File_Sharing_Simulation)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")

add_executable(peershare_simulator ${SOURCES})



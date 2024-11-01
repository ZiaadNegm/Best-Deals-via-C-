set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.15)

# Use vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "/home/ziaad/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")


project(CppWebScraper)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(scraper scraper.cpp)

# Find packages
find_package(cpr CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)

# Link libraries
target_link_libraries(scraper PRIVATE cpr::cpr LibXml2::LibXml2)

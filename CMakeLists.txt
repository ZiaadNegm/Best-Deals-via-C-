cmake_minimum_required(VERSION 3.15)

# Enable vcpkg manifest mode
set(VCPKG_FEATURE_FLAGS "manifests")

# Specify the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "/home/ziaad/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

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

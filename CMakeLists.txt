cmake_minimum_required(VERSION 4.1)

project(SoftwareRasterizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED) 
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCE "source/*.cpp" "source/*.h" ) 

add_executable(main ${SOURCE}) 

target_link_libraries(main PRIVATE SDL3::SDL3 Vulkan::Vulkan spdlog::spdlog)    




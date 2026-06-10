cmake_minimum_required(VERSION 4.1)

project(SoftwareRasterizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED) 
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)
find_package(assimp REQUIRED)

file(GLOB_RECURSE SOURCE "source/*.cpp" "source/*.h" ) 

add_executable(main ${SOURCE}) 


if(DEFINED Target_Platform)
    
    target_compile_definitions(main PRIVATE Target_Platform_${Target_Platform})
    message(STATUS "Target platform : ${Target_Platform}")

else()
    message(FATAL_ERROR "Add target platform in cmake presets!")

endif()


target_link_libraries(main PRIVATE SDL3::SDL3 Vulkan::Vulkan spdlog::spdlog assimp::assimp)    




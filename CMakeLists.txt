cmake_minimum_required(VERSION 3.14)

project(Nova-Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(GLM INTERFACE)
target_include_directories(GLM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)


find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES "Engine/src/*.cpp")
file(GLOB_RECURSE VENDOR_SOURCES "Vendor/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${ENGINE_SOURCES} ${VENDOR_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/Rendering
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/UI
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/UI/Object
                
                ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/include
                ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb_image/include
                )

target_link_libraries(${PROJECT_NAME} PRIVATE 
        glfw
        glad::glad 
        assimp::assimp 
        GLM
)

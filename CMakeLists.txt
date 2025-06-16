cmake_minimum_required(VERSION 3.14)

project(OpenGL_Project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLM (header-only)
add_library(GLM INTERFACE)
target_include_directories(GLM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)

# GLAD (статична бібліотека)
add_library(GLAD ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c)
target_include_directories(GLAD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/include)

# --- Assimp (налаштовуємо для Debug та Release) ---
add_library(assimp_imported INTERFACE IMPORTED)
target_include_directories(assimp_imported INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/bin)
set_target_properties(assimp_imported PROPERTIES
        IMPORTED_LIBNAME_DEBUG "assimp-vc143-mtd"
        IMPORTED_LIBNAME_RELEASE "assimp-vc143-mt"
        IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/bin/assimp-vc143-mtd.dll"
        IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/bin/assimp-vc143-mt.dll"
)

add_library(glfw_imported STATIC IMPORTED)
set_target_properties(glfw_imported PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/lib-vc2022/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include"
)


file(GLOB IMGUI_SOURCES "Vendor/imgui/src/*.cpp")

add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/include)
target_link_libraries(ImGui PUBLIC glfw_imported)

add_library(STB_Image STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb_image/src/stb_image.cpp)
target_include_directories(STB_Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb_image/include)


# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib)
# set(glfw3_DIR "D:/Installation/vcpkg/installed/x64-windows/share/glfw3")
# set(glad_DIR "D:/Installation/vcpkg/installed/x64-windows/share/glad")
# set(glm_DIR "D:/Installation/vcpkg/installed/x64-windows/share/glm")
# set(assimp_DIR "D:/Installation/vcpkg/installed/x64-windows/share/assimp")

# find_package(glfw3 CONFIG REQUIRED)
# find_package(glad CONFIG REQUIRED)
# find_package(glm CONFIG REQUIRED)
# find_package(assimp CONFIG REQUIRED)


file(GLOB_RECURSE ENGINE_SOURCES "Engine/src/*.cpp")

add_executable(OpenGL_Project 
                main.cpp 
                ${ENGINE_SOURCES}
)

target_include_directories(OpenGL_Project PRIVATE 
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/Rendering
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/UI
                ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/UI/Object
                
                ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/include
                ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb_image/include
                
                )

                
target_link_libraries(OpenGL_Project PRIVATE
        GLM        
        GLAD        
        glfw_imported       
        assimp_imported    
        ImGui    
        STB_Image 
)
# glfw glad::glad assimp::assimp





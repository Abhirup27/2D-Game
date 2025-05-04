cmake_minimum_required(VERSION 3.16)
# Update to C++20 for better compatibility with latest GCC
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE BUILD SYSTEMS DON'T ALWAYS RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build!" OFF)

# Runtime library settings - with GCC compatibility
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    # GCC/Clang equivalent settings
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

# Link-time optimization settings (supported by GCC)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Ensure SFML is built as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)

#for imgui-sfml
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui-docking/imgui")

# Compiler-specific optimizations
if(MSVC)
    add_compile_options(/arch:AVX2) # SIMD optimizations for MSVC
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC and Clang equivalent for AVX2
    add_compile_options(-mavx2)
    
    # Additional optimization flags for GCC
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -flto -march=native -ftree-vectorize)
    endif()
endif()

project(mygame)

# Add third-party libraries
add_subdirectory(thirdparty/SFML-3.0.1)
add_subdirectory(thirdparty/imgui-docking)
add_subdirectory(thirdparty/imgui-sfml-3.0)
add_subdirectory(thirdparty/glad)

# Source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Main executable
add_executable("${CMAKE_PROJECT_NAME}")
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 20)

# Resource path configuration
if(PRODUCTION_BUILD)
    # setup the ASSETS_PATH macro to be in the root folder of your exe
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
else()
    # This is useful to get an ASSETS_PATH in your IDE during development
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
endif()

# Add source files
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})

# Compiler-specific settings
if(MSVC)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
    # Uncomment to remove console in Windows
    # if(PRODUCTION_BUILD)
    #     set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    # endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Additional warnings for GCC/Clang
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -Wall -Wextra)
    
    # Position Independent Code - often needed for modern GCC
    set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# Include directories
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Link libraries
target_link_libraries("${CMAKE_PROJECT_NAME}" 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network
    imgui
    ImGui-SFML::ImGui-SFML
    glad
)

# Install rules (optional but useful for production)
if(PRODUCTION_BUILD)
    install(TARGETS "${CMAKE_PROJECT_NAME}" DESTINATION bin)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION bin)
endif()

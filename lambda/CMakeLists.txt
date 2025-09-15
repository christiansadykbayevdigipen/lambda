cmake_minimum_required(VERSION 3.11)

project(lambda)

set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan)
find_package(GLEW REQUIRED)

link_directories(${CMAKE_BINARY_DIR}/SDL/)

link_libraries(-lSDL3)

include_directories(${CMAKE_SOURCE_DIR}/SDL/include ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_definitions(-DDEBUG)

add_library(lambda SHARED src/window/window.cpp src/window/platform/shared/SharedPlatform.cpp src/application/application.cpp src/renderer/renderer.cpp src/renderer/api/VulkanRenderer.cpp src/renderer/api/OpenGLRenderer.cpp src/renderer/api/opengl/VertexArray.cpp src/renderer/api/opengl/VertexBuffer.cpp src/renderer/api/opengl/ShaderProgram.cpp)

target_link_libraries(lambda PRIVATE Vulkan::Vulkan GLEW::GLEW)
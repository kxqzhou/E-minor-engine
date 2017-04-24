
find_package( OpenGL REQUIRED )

# Add subdirectories to include dependency CMake projects

add_subdirectory(glew-2.0.0)
#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    link_libraries(${GLEW_LIBRARIES})
#endif()

add_subdirectory(glm)
add_subdirectory(glfw-3.2.1)


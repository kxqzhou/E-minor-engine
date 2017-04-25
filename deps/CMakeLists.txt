
find_package( OpenGL REQUIRED )
find_package( Threads REQUIRED )

# Add subdirectories to include dependency CMake projects

add_subdirectory(glew-2.0.0)
add_subdirectory(glfw-3.2.1)
add_subdirectory(soil)
add_subdirectory(freetype-2.7.1)


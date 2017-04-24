
# min version
cmake_minimum_required( VERSION 2.8 )

# project name
project( e_minor )

# add subdirectory
add_subdirectory( deps ) 

# Get all source files from src
aux_source_directory(src SRC_LIST)

# Get Include Directories
set (
      INCLUDE_DIRS
      include
      deps/glfw-3.2.1/include/GLFW
      deps/glew-2.0.0/include/GL
      deps/glm/glm
      deps/glm/gtc
      deps/glm/gtx
    )

include_directories(${INCLUDE_DIRS})

# Add executable to project
add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link GLFW and GLEW to main executable
target_link_libraries(${PROJECT_NAME} libglew_static glfw ${GLFW_LIBRARIES} )

# C++ Standard
add_definitions( -std=gnu++11 -m64 )

 
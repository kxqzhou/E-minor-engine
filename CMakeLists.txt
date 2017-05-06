
# special thanks to github.com/danielunderwood/glfw-game-engine for providing an example cmake

# min version
cmake_minimum_required( VERSION 2.8 )

# project name
project( e_minor )

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
option(BUILD_DEBUG     "Build with debug settings"    ON)

#-------------------------------------------------------------------------------
# Platform-specific settings
#-------------------------------------------------------------------------------

###################
# Building on OSX #
###################
if(APPLE)

  	# OSX Framework dependencies
	include_directories( "/System/Library/Frameworks" )
	find_library (COCOA_LIBRARIES Cocoa)
	find_library (IOKIT_LIBRARIES IOkit)
	find_library (COREVIDEO_LIBRARIES CoreVideo)

	# Clang configuration
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

	set(CLANG_CXX_FLAGS "-std=c++11 -m64")

	if(BUILD_DEBUG)
		set(CMAKE_BUILD_TYPE Debug)
		set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -g")
	else(BUILD_DEBUG)
		set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -O3")
		set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -funroll-loops")
		set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-narrowing")
		set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-deprecated-register")
	endif(BUILD_DEBUG)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")

	endif()

	# GCC configuration
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

	set(GCC_CXX_FLAGS "-std=gnu++11 -m64")

	if(BUILD_DEBUG)
		set(CMAKE_BUILD_TYPE Debug)
		set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -g")
	else(BUILD_DEBUG)
		set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -O3")
		set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -fopenmp")
	endif(BUILD_DEBUG)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")

	endif()

endif(APPLE)

##################
# Build on Linux #
##################
set(LINUX UNIX AND NOT APPLE)
if(LINUX)

  # GCC only
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

	set(GCC_CXX_FLAGS "-std=gnu++11 -m64")

    # X11 Dependencies
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXi")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXxf86vm")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXinerama")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXcursor")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXfixes")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrandr")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXext")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrender")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lX11")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lpthread")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lxcb")
	set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXau")

    # Debug configuration
    if(BUILD_DEBUG)
        set(CMAKE_BUILD_TYPE Debug)
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -g")
    else(BUILD_DEBUG)
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -O3")
        set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -fopenmp")
    endif(BUILD_DEBUG)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")

  endif()

endif(LINUX)



# add subdirectory
add_subdirectory( deps ) 

# Get all source files from src
aux_source_directory(src SRC_LIST)

# Get Include Directories
set (
      INCLUDE_DIRS
      include
      deps/glfw-3.2.1/include
      deps/glew-2.0.0/include
      deps/soil/src
      deps/freetype-2.7.1/include
    )

include_directories(${INCLUDE_DIRS})

# Add executable to project
add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link GLFW and GLEW to main executable
target_link_libraries(${PROJECT_NAME} libglew_static glfw ${GLFW_LIBRARIES} SOIL freetype )

# C++ Standard
#add_definitions( -std=c++0x )




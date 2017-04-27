
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "engine.h"

#include <cstdio>
#include <string>

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode ) {
	// close application when user presses escape
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
}

int main( int argc, char* argv[] ) {
	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// compatibility required for mac
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	GLuint screen_width = 800;
	GLuint screen_height = 600;
	if (argc == 3) {
		screen_width = std::stoi( argv[1] );
		screen_height = std::stoi( argv[2] );
		printf( "Using window size %d by %d", screen_width, screen_height );
	}

	GLFWwindow* window = glfwCreateWindow( screen_width, screen_height, "E-Minor Engine", NULL, NULL );
	if (!window) {
		printf( "Failed to create GLFW window\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	// init glew and catch errors
	glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    glfwSetKeyCallback( window, key_callback );

    // OpenGL configuration
    glViewport( 0, 0, screen_width, screen_height );
    glEnable( GL_DEPTH_TEST );

    // init engine

    // delta time vars
    GLfloat dt = 0.0f;
    GLfloat last_tick = 0.0f;

    // game loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// calc dt
    	GLfloat current_tick = glfwGetTime();
    	dt = current_tick - last_tick;
    	last_tick = current_tick;

		glfwPollEvents();

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glfwSwapBuffers( window );
	}

	glfwTerminate();
	return 0;
}


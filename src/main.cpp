
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "resource_manager.h"
#include "camera.h"

#include <cstdio>
#include <string>

using namespace glm;

// Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode ) {
	// close application when user presses escape
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
}

// Moves/alters the camera positions based on user input
void move_camera( GLFWwindow* window, GLfloat dt ) {
    // Camera controls
    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
        camera.ProcessKeyboard(FORWARD, dt);
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
        camera.ProcessKeyboard(BACKWARD, dt);
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
        camera.ProcessKeyboard(LEFT, dt);
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
        camera.ProcessKeyboard(RIGHT, dt);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}	

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}


int main( int argc, char* argv[] ) {
	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	// compatibility required for mac
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	GLuint screen_width = 800;
	GLuint screen_height = 600;
	if (argc == 3) {
		screen_width = std::stoi( argv[1] );
		screen_height = std::stoi( argv[2] );
		//printf( "Using window size %d by %d\n", screen_width, screen_height );
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

    // callback events
    glfwSetKeyCallback( window, key_callback );
    glfwSetCursorPosCallback( window, mouse_callback );
    glfwSetScrollCallback( window, scroll_callback );

    // OpenGL configuration
    glViewport( 0, 0, screen_width, screen_height );
    glEnable( GL_DEPTH_TEST );

    // init engine
    Engine engine( screen_width, screen_height );
    engine.init();

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
		move_camera( window, dt );

		// Create camera transformation
        mat4 view;
        view = camera.GetViewMatrix();
        mat4 projection;	
        projection = perspective(camera.Zoom, (float)screen_width / (float)screen_height, 0.1f, 1000.0f);

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glfwSwapBuffers( window );
	}

	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}


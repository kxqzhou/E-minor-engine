
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

int main (int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow* window = glfwCreateWindow( 800, 600, "E-Minor Engine", NULL, NULL );
	if (!window) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );

	glViewport( 0, 0, width, height );

	while ( !glfwWindowShouldClose( window ) ) {
		glfwPollEvents();
		glfwSwapBuffers( window );
	}

	glfwTerminate();
	return 0;
}

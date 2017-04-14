
#include <GL/glew.h>

#include <GLFW/glfw3.h>

int main (int argc, const char* argv[]) {
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwTerminate();
	return 0;
}



#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine {
public:
	GLuint width, height;

	Engine( GLuint w, GLuint h );
	~Engine();

	void init();

	void handle_input( GLfloat dt );
	void update( GLfloat dt );
	
	void render();
};


#endif

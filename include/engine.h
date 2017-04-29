

#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "renderer.h"

class Engine {
public:
	Engine( GLuint w, GLuint h, Camera* camera );
	~Engine();

	void init();

	void handle_input( GLfloat dt );
	void update( GLfloat dt );
	
	void render();
private:
	Renderer* renderer;
};


#endif

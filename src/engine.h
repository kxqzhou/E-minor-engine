

#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "camera.h"
#include "renderer.h"

enum class GenFunc { line, vert, tree };

class Engine {
public:
	GLuint width, height, cube_count;

	Engine( int w, int h, int cc=16 );
	~Engine();

	void go();

	void handle_input( GLfloat dt );
	void update( GLfloat dt );
	
	void render();

	void place_cubes( GenFunc f );
private:
	GLFWwindow* window;

	GLfloat light_pos[4] = { 0.0, 0.0, 0.0, 1.0 };

	void move_camera( GLfloat dt );

	Renderer* renderer;

	void init();

	std::vector<glm::vec3> cubePositions;

	glm::vec3 loc_m;

	GLfloat waver;
	GLfloat change;

	void branch( int seed, glm::vec3 pos, glm::vec3 dir );
};


#endif





#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "camera.h"

enum class GenFunc { line, vert, tree };

class Engine {
public:
	GLuint width, height, cube_count;

	Engine( GLuint w, GLuint h, Camera* cam, GLuint cc=16 );
	~Engine();

	void init( GenFunc f );

	void handle_input( GLfloat dt );
	void update( GLfloat dt );
	
	void render();

	void place_cubes( GenFunc f );
private:
	Camera* camera;
	GLuint VAO;
	std::vector<glm::vec3> cubePositions;
	glm::vec3 lightPos;

	void branch( int seed, glm::vec3 pos, glm::vec3 dir );
};


#endif

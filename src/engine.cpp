
#include "engine.h"

using namespace glm;

Engine::Engine( GLuint w, GLuint h, Camera* camera ) {
	renderer = new Renderer( w, h, camera );
}

Engine::~Engine() {
	delete renderer;
}

void Engine::init() {
	renderer->init();
}

void Engine::handle_input( GLfloat dt ) {
	
}

void Engine::update( GLfloat dt ) {
	
}

void Engine::render() {
	renderer->draw();
}


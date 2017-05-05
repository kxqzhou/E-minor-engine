
#include "engine.h"

using namespace glm;

Camera* camera = new Camera( vec3(0.0f, 0.0f, 3.0f) );
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

Engine::Engine( int w, int h, int cc ) {
	width = w;
	height = h;
	cube_count = cc;
}

void Engine::init() {
	camera->MovementSpeed = 0.3;

	loc_m = vec3(0.0f, 0.0f, 0.0f);

	waver = 1.1f;
    change = 0.01f;

	renderer = new Renderer( width, height, camera );
}

Engine::~Engine() {
	delete camera;
	delete renderer;
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode ) {
    // close application when user presses escape
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, GL_TRUE );
    }
}

void mouse_callback( GLFWwindow* window, double xpos, double ypos ) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}   

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ) {
    camera->ProcessMouseScroll(yoffset);
}

void Engine::go() {
	glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint( GLFW_SAMPLES, 4 );

	window = glfwCreateWindow( width, height, "E-Minor Engine", NULL, NULL );
    if (!window) {
        printf( "Failed to create GLFW window\n" );
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent( window );

    // init glew and catch errors
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    // needs to be after context 
    this->init();

    // callback events
    glfwSetKeyCallback( window, key_callback );
    glfwSetCursorPosCallback( window, mouse_callback );
    glfwSetScrollCallback( window, scroll_callback );

    // OpenGL configuration
    glViewport( 0, 0, width, height );
    glEnable( GL_DEPTH_TEST );
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    // text config
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // avoid seg faults for text..? feels dangerous
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // delta time vars
    GLfloat dt = 0.0f;
    GLfloat last_tick = 0.0f;

    glClearColor(.00f, 0.01f, 0.02f, 1.0f);

    // game loop
    while ( !glfwWindowShouldClose( window ) ) {
        // calc dt
        GLfloat current_tick = glfwGetTime();
        dt = current_tick - last_tick;
        last_tick = current_tick;

        // get input
        glfwPollEvents();
        move_camera( dt );
        this->handle_input( dt );

        // update state
        this->update( dt );

        // rendering
        glClearColor(.00f, 0.01f, 0.02f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        this->render();

        glfwSwapBuffers( window );
    }

    glfwTerminate();
}

void Engine::handle_input( GLfloat dt ) {
	if ( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS ) {
        renderer->bloom = !renderer->bloom;
    }
    if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
        loc_m.z += .1;
    if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
        loc_m.z -= .1;
    if ( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
        loc_m.x += .1;
    if ( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
        loc_m.x -= .1;
    // Change parallax height scale
    if ( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS)
        renderer->exposure -= 0.5 * dt;
    else if ( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS)
        renderer->exposure += 0.5 * dt;
}

// Moves/alters the camera positions based on user input
void Engine::move_camera( GLfloat dt ) {
    // Camera controls
    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
        camera->ProcessKeyboard(FORWARD, dt);
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
        camera->ProcessKeyboard(BACKWARD, dt);
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
        camera->ProcessKeyboard(LEFT, dt);
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
        camera->ProcessKeyboard(RIGHT, dt);
    /*if ( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS ) 
        camera->ProcessKeyboard(UP, dt);
    if ( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS ) 
        camera->ProcessKeyboard(DOWN, dt);*/
}

void Engine::update( GLfloat dt ) {
	if (waver > 1.5) {
        change = -.01;
    } else if (waver < .5){
        change = .01;
    }
    waver += change;
}

void Engine::render() {
	renderer->drawFloor();
    renderer->drawCubes();
    renderer->drawText( "E-Minor", width / 2, height * 7 / 8 );
    renderer->drawLights( waver );
}


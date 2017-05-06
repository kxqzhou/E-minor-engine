
#include "engine.h"

using namespace glm;

Camera* camera = new Camera( vec3(0.0f, 0.0f, 3.0f) );
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

Engine::Engine( int w, int h, int cc ) {
	width = w;
	height = h;
	cube_count = cc;

    place_cubes( GenFunc::tree );
}

void Engine::init() {
	camera->MovementSpeed = 10;

	loc_m = vec3(0.0f, 0.0f, 0.0f);

	waver = 1.1f;
    change = 0.01f;

    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, 20.0); 

	renderer = new Renderer( width, height, camera );

    srand( time(NULL) );
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
    // lighting
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
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

        // lights
        light_pos[0] = camera->Position[0];
        light_pos[1] = camera->Position[1];
        light_pos[2] = camera->Position[2];
        glLightfv( GL_LIGHT0, GL_POSITION, light_pos );

        light_pos[0] = camera->Front[0];
        light_pos[1] = camera->Front[1];
        light_pos[2] = camera->Front[2];
        glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, light_pos );

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
    if ( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS ) {
        this->place_cubes( GenFunc::tree );
    }
    if ( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS ) {
        this->place_cubes( GenFunc::vert );
    }
    if ( glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS ) {
        this->place_cubes( GenFunc::line );
    }
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
    if ( glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS ) 
        camera->ProcessKeyboard(UP, dt);
    if ( glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS ) 
        camera->ProcessKeyboard(DOWN, dt);
}

void Engine::branch( int seed, vec3 pos, vec3 dir ) {
    if (seed == 1) {
        return;
    }
    
    // roulette terminate, higher chance to stop the farther along the branch
    if ( rand() / (float)RAND_MAX > 1.0 / seed ) {
        vec3 next_pos = dir + pos;

        cubePositions.push_back( next_pos );

        // calc vec orthagonal to current
        float x = rand() / (float)RAND_MAX * 2 - 1;
        float y = rand() / (float)RAND_MAX * 2 - 1;
        float z = (dir[0] * x + dir[1] * y) / dir[2];
        branch( seed / 4, next_pos, normalize( vec3(x, y, z) ) );

        branch( seed / 2, next_pos, dir );
    }
}

vec3 linear(int i, float z) {
    return vec3(i, 0.0f, z);
}

vec3 vertical(int i, float x, float z) {
    return vec3(x, i, z);
}

void Engine::place_cubes( GenFunc f ) {
    //vec3 (*gen_func)(int);

    if (f == GenFunc::line) {
        float z = rand() / (float)RAND_MAX * 50 - 25;

        for (int i = 0; i < cube_count; i++) {
            cubePositions.push_back( linear(i, z) );
        }
    }
    else if (f == GenFunc::vert ) {
        float x = rand() / (float)RAND_MAX * 100 - 50;
        float z = rand() / (float)RAND_MAX * 50 - 25; 

        for (int i = 0; i < cube_count; i++) {
            cubePositions.push_back( vertical(i, x, z) );
        }
    }
    else if (f == GenFunc::tree) {
        for (int i = 0; i < cube_count; i++) {
            vec3 pos( rand() / (float)RAND_MAX * 100 - 50, 0, rand() / (float)RAND_MAX * 50 - 25 );
            for (int j = 1; j < rand() / (float)RAND_MAX * (cube_count / 2) + (cube_count / 2); j++) {
                cubePositions.push_back( vec3(pos[0], j, pos[2]) );
            }
        }

        for (int i = 0; i < cube_count; i++) {
            vec3 dir( rand() / (float)RAND_MAX * 2 - 1, 0, rand() / (float)RAND_MAX * 2 - 1);
            for (int j = 1; j < cube_count; j++) {
                branch( cube_count, cubePositions[i * cube_count + j], normalize(dir) );
            }
        }
    }
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
    renderer->drawText( "E-Minor", width / 2, height * 7 / 8 );
	renderer->drawFloor();
    renderer->drawCubes( cubePositions );
    renderer->drawLights( waver );
}


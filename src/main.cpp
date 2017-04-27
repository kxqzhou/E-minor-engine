
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
//#include "resource_manager.h"
#include "camera.h"
#include "Zader.h"

#include <cstdio>
#include <string>

using namespace glm;

// Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//Light position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);



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

    // Setup and compile our shaders
    Shader ourShader("../res/shaders/transform.vs", "../res/shaders/transform.frag");
    Shader lightingShader("../res/shaders/basic_lighting.vs", "../res/shaders/basic_lighting.frag");


    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // game loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// calc dt
    	GLfloat current_tick = glfwGetTime();
    	dt = current_tick - last_tick;
    	last_tick = current_tick;

		glfwPollEvents();
		move_camera( window, dt );


        glClearColor( 0.922f, 0.72f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Draw our first triangle
        ourShader.Use();

        lightingShader.Use();
        GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(lightingShader.Program, "lightColor");
        GLint lightPosLoc    = glGetUniformLocation(lightingShader.Program, "lightPos");
        GLint viewPosLoc     = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);

        // Create camera transformation
        mat4 view;
        view = camera.GetViewMatrix();
        mat4 projection;
        projection = perspective(camera.Zoom, (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
        // Get the uniform locations

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lightingShader.Program,  "view");
        GLint projLoc  = glGetUniformLocation(lightingShader.Program,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        for(int i = 0; i < 10; i++) {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate(model, vec3(1.0f+i, 1.0f+i, 1.0f*i));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        for(GLuint i = 0; i < 10; i++) {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate(model, vec3(-1.0f+i, -1.0f+i, -1.0f*i));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

		glfwSwapBuffers( window );
	}

	//ResourceManager::Clear();
	glfwTerminate();
	return 0;
}


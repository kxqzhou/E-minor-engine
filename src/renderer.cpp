
#include "renderer.h"
#include "resource_manager.h"

using namespace glm;

Renderer::Renderer( GLuint w, GLuint h, Camera* cam ) {
	width = w;
	height = h;
	camera = cam;
	lightPos = vec3( 1.2f, 1.0f, 2.0f );
}

Renderer::~Renderer() {

}

void Renderer::draw() {
	// Create camera transformation
    mat4 view;
    view = camera->GetViewMatrix();
    mat4 projection;   
    projection = perspective(camera->Zoom, (float)width/(float)height, 0.1f, 1000.0f);

    for (auto &p : ResourceManager::Shaders) {
    	Shader shader = p.second;

    	shader.Use();
	    GLint objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
	    GLint lightColorLoc  = glGetUniformLocation(shader.ID, "lightColor");
	    GLint lightPosLoc    = glGetUniformLocation(shader.ID, "lightPos");
	    GLint viewPosLoc     = glGetUniformLocation(shader.ID, "viewPos");
	    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	    glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
	    glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
	    glUniform3f(viewPosLoc,     camera->Position.x, camera->Position.y, camera->Position.z);
	    
	    
	    // Get the uniform locations
	    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	    GLint viewLoc  = glGetUniformLocation(shader.ID,  "view");
	    GLint projLoc  = glGetUniformLocation(shader.ID,  "projection");
	    // Pass the matrices to the shader
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
	    
	    glBindVertexArray(VAO);

	    for(GLuint i = 0; i < cubePositions.size(); i++) {
	        // Calculate the model matrix for each object and pass it to shader before drawing
	        mat4 model;
	        model = translate(model, cubePositions[i]);
	        GLfloat angle = 20.0f * i; 
	        model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));
	        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	        glDrawArrays(GL_TRIANGLES, 0, 36);          
	    }
	    glBindVertexArray(0);
    }
}

void Renderer::init() {
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
    
    vec3 positions[] = {
        vec3(0.0f, 0.0f, 0.0f), 
        vec3(2.0f, 5.0f, -15.0f), 
        vec3(-1.5f, -2.2f, -2.5f),  
        vec3(-3.8f, -2.0f, -12.3f),  
        vec3(2.4f, -0.4f, -3.5f),  
        vec3(-1.7f, 3.0f, -7.5f),  
        vec3(1.3f, -2.0f, -2.5f),  
        vec3(1.5f, 2.0f, -2.5f), 
        vec3(1.5f, 0.2f, -1.5f), 
        vec3(-1.3f, 1.0f, -1.5f)  
    };
    cubePositions = std::vector<vec3>(positions, positions + 10);

	GLuint VBO;
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

    ResourceManager::LoadShader( "../res/shaders/basic_lighting.vs", 
    							 "../res/shaders/basic_lighting.frag", NULL, "lighting" );
}



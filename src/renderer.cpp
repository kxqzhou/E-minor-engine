

#include "renderer.h"

using namespace glm;
using Texture::loadTexture;

Renderer::Renderer( int w, int h, Camera* cam ) {
	width = w;
	height = h;
	camera = cam;

	// shaders
	shads["tex"] = Shader( "tex.vs", "tex.frag" );
	shads["bloom"] = Shader("bloom.vs", "bloom.frag");
    shads["light"] = Shader("bloom.vs", "light_box.frag");
    shads["blur"] = Shader("blur.vs", "blur.frag");
    shads["bloomFinal"] = Shader("bloom_final.vs", "bloom_final.frag");

    // Set samplers
    shads["bloomFinal"].use();
    glUniform1i(glGetUniformLocation(shads["bloomFinal"].ID, "scene"), 0);
    glUniform1i(glGetUniformLocation(shads["bloomFinal"].ID, "bloomBlur"), 1);

    // textures
    textures["floor"] = loadTexture("overlook.png");
    textures["pato"] = loadTexture("pato.png");

	// lighting
	bloom = true;
	exposure = 0.2f; // Change with Q and E

	// Light sources
    // - Positions
	srand(time(NULL));
	for (int i = 0; i < 20 ; i++) {
        float x = (-25.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (200.0 - (-25.0 + 1));
        float y = (0.2 + 1) + (((float) rand()) / (float) RAND_MAX) * (4.0 - (0.2 + 1));
        float z = (-25.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (15.0 - (-15.0 + 1));
        rand_positions.push_back(vec3(x,y,z));
    }

    for (int i = 0; i < 2 ; i++) {
        float x = (25.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (200.0 - (25.0 + 1));
        float y = (0.2 + 1) + (((float) rand()) / (float) RAND_MAX) * (5.0 - (0.2 + 1));;
        float z = (-25.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (15.0 - (-15.0 + 1));;
        slow_po.push_back(vec3(x,y,z));
    }

    // - Colors
    for (int i = 0; i < 20 ; i++) {
        float x = (0.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (5.0 - (0.0 + 1));
        float y = (0.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (2.0 - (0.0 + 1));
        float z = (0.0 + 1) + (((float) rand()) / (float) RAND_MAX) * (3.0 - (0.0 + 1));
        rand_lights.push_back(vec3(x,y,z));
    }

	// freetype stuff
    if (FT_Init_FreeType(&ft)) {
        printf("Error::FreeType: Could not initialize FreeType library\n");
    }

    if (FT_New_Face(ft, "../res/fonts/FreeSans.ttf", 0, &face)) {
        printf("Error::FreeType: Failed to load font\n");
    }

    FT_Set_Pixel_Sizes(face, 0, 48); 

    if (FT_Load_Char(face, 'E', FT_LOAD_RENDER))
        printf("Error::FreeType: Failed to load glyph\n");

    // --------- cube vao vbo ---------
    GLfloat vertices[] = {
	        // Back face
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
	        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
	        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
	        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
	        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
	        // Front face
	        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
	        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
	        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
	        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
	        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
	        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
	        // Left face
	        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
	        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
	        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
	        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
	        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	        // Right face
	        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
	        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
	        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
	        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
	        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
	        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
	        // Bottom face
	        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
	        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
	        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
	        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
	        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
	        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
	        // Top face
	        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
	        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
	        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
	        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
	};
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	// Fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    // --------------------------------

	// -------- quad vao vbo ----------
	quadVAO = 0;
	GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // Setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    // --------------------------------

    // Set up floating point framebuffer to render scene to
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // - Create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    glGenTextures(2, colorBuffers);
    for (GLuint i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);  // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer not complete!\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Ping pong framebuffer for blurring
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (GLuint i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // Also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("Framebuffer not complete!\n");
    }
}

Renderer::~Renderer() {
	shads.clear();

	FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Renderer::drawText( std::string text, int pen_x, int pen_y ) {
    FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */

    FT_UInt       glyph_index;
    FT_Bool       use_kerning;
    FT_UInt       previous;

    use_kerning = FT_HAS_KERNING( face );
    previous    = 0;

    for ( int n = 0; n < text.size(); n++ ) {
      // convert character code to glyph index

      glyph_index = FT_Get_Char_Index( face, text[n] );

      // retrieve kerning distance and move pen position
      if ( use_kerning && previous && glyph_index )
      {
        FT_Vector  delta;


        FT_Get_Kerning( face, previous, glyph_index,
                        FT_KERNING_DEFAULT, &delta );

        pen_x += delta.x >> 6;
      }

      // load glyph image into the slot (erase previous one)
      if ( FT_Load_Glyph( face, glyph_index, FT_LOAD_RENDER ) )
        continue;  // ignore errors

      // now draw to our target surface
      my_draw_bitmap( &slot->bitmap,
                      pen_x + slot->bitmap_left,
                      pen_y - slot->bitmap_top );

      // increment pen position
      pen_x += slot->advance.x >> 6;

      // record current glyph index
      previous = glyph_index;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::my_draw_bitmap( FT_Bitmap* bm, int top_left_x, int top_left_y ) {
    GLfloat xpos = top_left_x;
    GLfloat ypos = top_left_y;

    GLfloat w = bm->width;
    GLfloat h = bm->rows;

    // Update VBO for each character
    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 },            
        { xpos,     ypos,       0.0, 1.0 },
        { xpos + w, ypos,       1.0, 1.0 },

        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos + w, ypos + h,   1.0, 0.0 }           
    };

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        bm->width,
        bm->rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bm->buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // render
    Shader tex = shads["tex"];
    mat4 projection = ortho(0.0f, (float)width, 0.0f, (float)height);
    tex.use();
    glUniformMatrix4fv(glGetUniformLocation(tex.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUniform3f(glGetUniformLocation(tex.ID, "textColor"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, texture);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Renderer::renderCube() {
	glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Renderer::renderQuad() {
	glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::drawFloor() {
	Shader shader = shads["bloom"];

	// Render scene into floating point framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 projection = perspective(camera->Zoom, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
    mat4 view = camera->GetViewMatrix();
    mat4 model;
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, value_ptr(view));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures["floor"]);
    // - set lighting uniforms
    for (GLuint i = 0; i < rand_positions.size(); i++) {
        glUniform3fv(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].Position").c_str()),
                     1, &rand_positions[i][0]);
        glUniform3fv(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].Color").c_str()), 1,
                     &rand_lights[i][0]);
    }
    for (GLuint i = 0; i < slow_po.size(); i++) {
        glUniform3fv(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].Position").c_str()),
                     1, &slow_po[i][0]);
        glUniform3fv(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].Color").c_str()), 1,
                     &rand_lights[i][0]);
    }
    glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, &camera->Position[0]);

    // - create one large cube that acts as the floor
    model = mat4();
    model = translate(model, vec3(0.0f, -4.0f, 0.0));
    model = scale(model, vec3(70.0f, 1.0f, 70.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();
    // - create one large cube that acts as the floor
    model = mat4();
    model = translate(model, vec3(70.0f, -3.0f, 0.0));
    model = scale(model, vec3(70.0f, 1.0f, 70.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();
    // - create one large cube that acts as the floor
    model = mat4();
    model = translate(model, vec3(140.0f, -2.0f, 0.0));
    model = scale(model, vec3(70.0f, 1.0f, 70.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();
    // - create one large cube that acts as the floor
    model = mat4();
    model = translate(model, vec3(210.0f, -1.0f, 0.0));
    model = scale(model, vec3(70.0f, 1.0f, 70.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    // - then create multiple cubes as the scenery
    glBindTexture(GL_TEXTURE_2D, textures["pato"]);
    model = mat4();
    model = translate(model, vec3(0.0f, 0.5f, 0.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    model = mat4();
    model = translate(model, vec3(2.0f, 0.0f, 1.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    model = mat4();
    model = translate(model, vec3(-1.0f, -1.0f, 2.0));
    //model = rotate(model, 60.0f, normalize(vec3(1.0, 0.0, 1.0)));
    model = scale(model, vec3(2.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    model = mat4();
    model = translate(model, vec3(0.0f, 2.7f, 4.0));
    //model = rotate(model, 23.0f, normalize(vec3(1.0, 0.0, 1.0)));
    model = scale(model, vec3(2.5));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawLights( float waver ) {
	// - finally show all the light sources as bright cubes
	Shader shaderLight = shads["light"];
    shaderLight.use();
    mat4 projection = perspective(camera->Zoom, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
    mat4 view = camera->GetViewMatrix();
    mat4 model;
    glUniformMatrix4fv(glGetUniformLocation(shaderLight.ID, "projection"), 1, GL_FALSE,
                       value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderLight.ID, "view"), 1, GL_FALSE, value_ptr(view));

    for (GLuint i = 0; i < rand_positions.size(); i++) {
        model = mat4();
        vec3 npos = rand_positions[i];
        model = translate(model,vec3(npos.x, npos.y*waver, npos.z) );
        model = scale(model, vec3(0.8f));
        glUniformMatrix4fv(glGetUniformLocation(shaderLight.ID, "model"), 1, GL_FALSE, value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderLight.ID, "lightColor"), 1, &rand_lights[i][0]);
        renderCube();
    }
    for (GLuint i = 0; i < slow_po.size(); i++) {
        model = mat4();
        model = translate(model,slow_po[i] );
        model = scale(model, vec3(0.8f));
        glUniformMatrix4fv(glGetUniformLocation(shaderLight.ID, "model"), 1, GL_FALSE, value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderLight.ID, "lightColor"), 1, &rand_lights[i][0]);
        renderCube();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Blur bright fragments w/ two-pass Gaussian Blur
    GLboolean horizontal = true, first_iteration = true;
    GLuint amount = 14;
    Shader shaderBlur = shads["blur"];
    shaderBlur.use();
    for (GLuint i = 0; i < amount; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        glUniform1i(glGetUniformLocation(shaderBlur.ID, "horizontal"), horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1]
                                                     : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader shaderBloomFinal = shads["bloomFinal"];
    shaderBloomFinal.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    //glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
    glUniform1i(glGetUniformLocation(shaderBloomFinal.ID, "bloom"), bloom);
    glUniform1f(glGetUniformLocation(shaderBloomFinal.ID, "exposure"), exposure);
    renderQuad();
}

void Renderer::drawCubes() {
    mat4 model;

    // generate 'player' cube
    //vec3 mode_l(camera->Front.x , camera->Position.y, camera->Position.z - 5);
    vec3 mode_l = camera->Position + 5.0f * normalize(camera->Front);
    model = translate(model, vec3(mode_l.x, mode_l.y - 2, mode_l.z));
    for (int j = 0; j < rand_positions.size(); j++){

        if (abs(rand_positions[j].x - mode_l.x) < 2. &&
            abs(rand_positions[j].y - mode_l.y) < 2. &&
            abs(rand_positions[j].z - (mode_l.z-2)) < 2.){
            //rand_positions.clear();
            rand_positions.erase((rand_positions.begin()+j));
            printf("found a light!\n");
        }
    }
    for (int j = 0; j < slow_po.size(); j++) {
        if (abs(slow_po[j].x - mode_l.x) < 2. &&
            abs(slow_po[j].y - mode_l.y) < 2. &&
            abs(slow_po[j].z - (mode_l.z - 2)) < 2.) {
            //rand_positions.clear();
            camera->MovementSpeed = .10;
            slow_po.erase((slow_po.begin() + j));
            printf("found a light!\n");
        }
    }
    //model = rotate(model, 124.0f, normalize(vec3(1.0, 0.0, 1.0)));
    model = scale(model, vec3(1.0));
    glUniformMatrix4fv(glGetUniformLocation(shads["bloom"].ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();

    renderCube();
    model = mat4();
    model = translate(model, vec3(-3.0f, 0.0f, 0.0));
    glUniformMatrix4fv(glGetUniformLocation(shads["bloom"].ID, "model"), 1, GL_FALSE, value_ptr(model));
    renderCube();
}



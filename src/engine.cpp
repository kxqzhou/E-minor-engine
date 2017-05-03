
#include "engine.h"
#include "resource_manager.h"

#include <cstdio>
#include <string>

using namespace glm;

Engine::Engine( GLuint w, GLuint h, Camera* cam, GLuint cc ) {
	width = w;
	height = h;
	camera = cam;
	lightPos = vec3( 1.2f, 1.0f, 2.0f );
	cube_count = cc;
}

Engine::~Engine() {
	FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Engine::init( GenFunc f ) {
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
    
    place_cubes( f );

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

    ResourceManager::LoadShader( "../res/shaders/tex.vs", 
                                 "../res/shaders/tex.frag", NULL, "tex" );
}

vec3 linear(int i) {
	return vec3(i, 0.0f, 0.0f);
}

vec3 vertical(int i) {
	return vec3(0.0f, i, 0.0f);
}

void Engine::branch( int seed, vec3 pos, vec3 dir ) {
    if (seed == 0) {
        return;
    }

    // roulette terminate
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

void Engine::place_cubes( GenFunc f ) {
	vec3 (*gen_func)(int);

    if (f == GenFunc::line || f == GenFunc::vert) {
        gen_func = (f == GenFunc::line) ? linear : vertical;

        for (int i = 0; i < cube_count; i++) {
            cubePositions.push_back( gen_func(i) );
        }
    }
    else if (f == GenFunc::tree) {
        srand( time(NULL) );

        for (int i = 0; i < cube_count; i++) {
            vec3 pos( rand() / (float)RAND_MAX * 50 - 25, 0, rand() / (float)RAND_MAX * 50 - 25 );
            for (int j = 1; j < cube_count; j++) {
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

    /*
	switch( f ) {
		case GenFunc::line :
			gen_func = linear;
			break;
		case GenFunc::vert :
			gen_func = vertical;
			break;
	}
    */
}

void Engine::handle_input( GLfloat dt ) {
	
}

void Engine::update( GLfloat dt ) {
	/*for (int i = 0; i < cubePositions.size(); i++) {
		cubePositions[i][1] += dt;
	}*/
}

void Engine::render() {
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

    renderText();
}

void my_draw_bitmap( FT_Bitmap* bm, int top_left_x, int top_left_y ) {
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
    Shader tex = ResourceManager::GetShader("tex");
    mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f);
    tex.Use();
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

void Engine::renderText() {
    FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
    FT_UInt       glyph_index;
    FT_Bool       use_kerning;
    FT_UInt       previous;
    int           pen_x, pen_y, n;

    pen_x = width / 2;
    pen_y = height / 8;

    use_kerning = FT_HAS_KERNING( face );
    previous    = 0;

    // change to mapping later
    std::string text = "E-Minor";

    for ( n = 0; n < text.size(); n++ ) {
      /* convert character code to glyph index */
      glyph_index = FT_Get_Char_Index( face, text[n] );

      /* retrieve kerning distance and move pen position */
      if ( use_kerning && previous && glyph_index )
      {
        FT_Vector  delta;


        FT_Get_Kerning( face, previous, glyph_index,
                        FT_KERNING_DEFAULT, &delta );

        pen_x += delta.x >> 6;
      }

      /* load glyph image into the slot (erase previous one) */
      if ( FT_Load_Glyph( face, glyph_index, FT_LOAD_RENDER ) )
        continue;  /* ignore errors */

      /* now draw to our target surface */
      my_draw_bitmap( &slot->bitmap,
                      pen_x + slot->bitmap_left,
                      pen_y - slot->bitmap_top );

      /* increment pen position */
      pen_x += slot->advance.x >> 6;

      /* record current glyph index */
      previous = glyph_index;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


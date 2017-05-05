
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <string>

namespace Texture {
    GLuint loadTexture(std::string path);
};

#endif


#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TexCoord;

uniform vec4 Color;

uniform mat4 projection;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
	vColor = Color;
	vTexCoord = TexCoord;
	gl_Position = projection * vec4(position, 1.0);
}

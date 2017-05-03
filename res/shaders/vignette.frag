#version 330 core

uniform sampler2D u_tex;

uniform vec2 resolution;

in vec4 vColor;
in vec2 vTexCoord;

//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
const float RADIUS = 0.5;

//softness of our vignette, between 0.0 and 1.0
const float SOFTNESS = 0.2;

void main() {
	vec4 texColor = texture2D(u_tex, vTexCoord);
	
	//determine center position
	vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5);
	
	//determine the vector length of the center position
	float len = length(position);
	
	//use smoothstep to create a smooth vignette
	float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);
	
	//apply the vignette with 50% opacity
	texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.5);

	gl_FragColor = texColor * vColor;
}

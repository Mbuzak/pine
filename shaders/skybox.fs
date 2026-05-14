#version 150 core

in vec4 fragPos;
out vec4 outColor;

uniform samplerCube tex_skybox;

void main() {
	outColor = 0.3 * texture(tex_skybox, fragPos.xyz);
}

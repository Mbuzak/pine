#version 150 core

out vec4 outColor;

void main() {
	vec3 color_outline = vec3(0.0, 0.0, 0.35);
	outColor = vec4(color_outline, 1.0);
}

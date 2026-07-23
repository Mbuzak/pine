#version 330 core

layout (location = 0) in vec4 inPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec4 fragPos;

void main() {
	gl_Position = projection * view * model * inPosition;
	fragPos = inPosition;
}

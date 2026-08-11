#version 330 core

layout (location = 0) in vec4 inPosition;

layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

uniform mat4 matModel;

void main() {
	gl_Position = projection * view * matModel * inPosition;
}

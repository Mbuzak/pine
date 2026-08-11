#version 330 core
layout (location = 0) in vec4 inPosition;

layout (std140) uniform MatricesLight {
    mat4 projection_light;
    mat4 view_light;
};

uniform mat4 matModel;

void main() {
    gl_Position = projection_light * view_light * matModel * inPosition;
}

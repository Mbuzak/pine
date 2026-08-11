#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

layout (std140) uniform MatricesLight {
	mat4 projection_light;
	mat4 view_light;
};

uniform mat4 matModel;

out VertexData {
	vec4 position;
	vec2 uv;
	vec3 normal;

	vec4 position_light;
} out_data;

void main() {
	out_data.position = matModel * inPosition;
	out_data.uv = inUV;
	mat3 matNormal = mat3(transpose(inverse(matModel)));
	out_data.normal = normalize(matNormal * inNormal);
	out_data.position_light = projection_light * view_light * matModel * inPosition;

	gl_Position = projection * view * matModel * inPosition;
}

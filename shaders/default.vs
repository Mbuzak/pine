#version 330 core

// Input
// Atrybuty wierzcholkow z VAO
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out VertexData {
	vec4 position;
	vec2 uv;
	vec3 normal;

	vec4 position_light;
} out_data;

// Uniform
// Macierz rzutowania i transformacji
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

uniform mat3 matNormal;

uniform mat4 lightProj;
uniform mat4 lightView;

void main() {
	out_data.position = matModel * inPosition;
	out_data.uv = inUV;
	out_data.normal = normalize(matNormal * inNormal);
	out_data.position_light = lightProj * lightView * matModel * inPosition;

	// Ostateczna pozycja wierzcholka
	gl_Position = matProj * matView * matModel * inPosition;
}

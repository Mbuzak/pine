#version 330 core

// Atrybuty wierzcholkow z VAO
layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;
layout( location = 2 ) in vec3 inNormal;

// Macierz rzutowania i transformacji
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

uniform mat3 matNormal;

// Dane wyjsciowe do shadera fragmentow
out vec4 Position;
out vec2 UV;
out vec3 Normal;

void main() {
	// Przekazanie danych do shadera fragmentow
	Position = matModel * inPosition;
	UV = inUV;
	//mat3 matNormal = mat3( transpose( inverse( matModel ) ) );
	Normal = normalize( matNormal * inNormal );

	// Ostateczna pozycja wierzcholka
	gl_Position = matProj * matView * matModel * inPosition;
}

#version 150 core

// zmienne wejsciowe
in vec4 inoutPos;
in vec3 inoutColor;
in vec2 inoutUV;

//uniform float Time = 0.0;
//vec2 newUV = inoutUV;

// uchwyt tekstury
uniform sampler2D tex;

// zmienna wyjsciowa
out vec4 outColor;


void main()
{
	// pobranie wartosci tekstury dla zadanej
	// wspolrzednej UV

	vec4 texColor = texture(tex, inoutUV);

	if ( texColor.r > 0.75 && texColor.g < 0.5 && texColor.b < 0.5 ) {
		discard;
	}
	//vec4 texColor = texture(0, 0, 0, 0);
	//vec4 texColor = texture(0.0, 0.0, 0.0, inoutUV);

	outColor = texColor;
}

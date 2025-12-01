#version 150 core

// zmienne wejsciowe
in vec4 inoutPos;
in vec3 inoutColor;
in vec2 inoutUV;

//uniform float Time = 0.0;
//vec2 newUV = inoutUV;

// uchwyt tekstury
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;

// zmienna wyjsciowa
out vec4 outColor;


void main()
{
	// pobranie wartosci tekstury dla zadanej
	// wspolrzednej UV
	vec4 texColor = texture(tex, inoutUV) * 0.7 + texture(tex2, inoutUV) * 0.4 +  + texture(tex3, inoutUV) * 0.4;
	//vec4 texColor = texture(0, 0, 0, 0);
	//vec4 texColor = texture(0.0, 0.0, 0.0, inoutUV);

	outColor = texColor;

	//vec4 tc1 = texture(tex, Texcoord);
    //vec4 tc2 = texture(tex2, Texcoord);
    //outColor = mix(tc1, tc2, 0.5);
}

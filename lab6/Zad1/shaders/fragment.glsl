#version 150 core

#define NUMBER_OF_LIGHTS 7

#define POINT_LIGHT 0
#define DIRECTION_LIGHT 1

#define PHONG_REFLECTION_LIGHT false
#define BLINN_PHONG_REFLECTION_LIGHT true

// Kolor ostateczny
out vec4 outColor;

// Dane wejsciowe z shadera wierzcholkow
in vec4 Position;
in vec2 UV;
in vec3 Normal;

// Pozycja kamery przekazana z aplikacji
uniform vec3 cameraPos;

// flags
uniform bool reflectionLight;
uniform bool Light;
uniform bool Direction;

// uchwyt tekstury
uniform sampler2D tex;
uniform bool hasTex;

// model
uniform bool isLight;
uniform vec3 lightModelColor;

// Struktura oswietlenia punktowego
struct LightParam {
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Attenuation;
	vec3 Position; // Direction dla kierunkowego
};

uniform LightParam dirLight;
uniform LightParam myLight;

uniform LightParam lights[NUMBER_OF_LIGHTS];

struct MaterialParam {
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

uniform MaterialParam myMaterial;

vec3 calculateLight(vec4 Position, vec3 Normal, LightParam light, MaterialParam material, int mode) {
	vec3 lightCoef = vec3(0.3);

	if ( Light == false ) {
		return lightCoef;
	}

	// ambient
	vec3 AmbientPart = light.Ambient * material.Ambient;

	// diffuse
	vec3 L = vec3(0.0);

	if( mode == DIRECTION_LIGHT ) L = -light.Position;
	else L = normalize(light.Position - Position.xyz);

	float diff = max(dot(L, Normal), 0);
	vec3 DiffusePart = diff * light.Diffuse * material.Diffuse;

	// specular
	float spec = 1.0;
	vec3 E = normalize(cameraPos - Position.xyz);

	if ( reflectionLight == PHONG_REFLECTION_LIGHT ) {
		vec3 R = reflect(-E, Normal);
		spec = pow(max(dot(R, L), 0), material.Shininess);
	}
	else {
		vec3 H = normalize( L + E );
		spec = pow( max( dot( H, Normal ), 0 ), material.Shininess );
	}

	vec3 SpecularPart = spec * light.Specular * material.Specular;

	float LV = distance(Position.xyz, light.Position);
	float Latt = 1 / (light.Attenuation.x + light.Attenuation.y * LV + light.Attenuation.z * LV * LV);

	// Glowny wzor na swiatlo
	lightCoef = AmbientPart + Latt * ( DiffusePart + SpecularPart );

	return lightCoef;
}

void main() {
	if ( isLight ) {
		outColor = vec4(lightModelColor, 1.0);
	}
	else {
		// odleglosc od poczatku ukladu wspolrzednych
		float dist = length( Position );
		//vec3 crazyColor = vec3(sin(dist*3.0)/2.0 + 0.5, sin(Position.y)/2.0 + 0.5, 0.5);
		vec3 crazyColor = vec3(0.1, 0.3, 0.1);

		// Kolor poczatkowy - tutaj mozna dodac kolor z tekstury
		vec3 Color = crazyColor;

		if ( hasTex ) {
			vec4 texColor = texture(tex, UV);
			Color += texColor.xyz;
		}

		vec3 lcc = vec3(0.0);

		if ( Direction ) lcc += calculateLight(Position, Normal, dirLight, myMaterial, DIRECTION_LIGHT);
		else lcc += calculateLight(Position, Normal, dirLight, myMaterial, POINT_LIGHT);

		for( int i = 0 ; i < NUMBER_OF_LIGHTS ; i++ ) {
			lcc += calculateLight(Position, Normal, lights[i], myMaterial, POINT_LIGHT);
		}

		// Obliczanie koloru ostatecznego
		vec3 finalColor = lcc * Color;
		
		outColor = vec4(finalColor, 1.0);
	}
}

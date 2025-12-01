#version 150 core

#define NUMBER_OF_LIGHTS 4

#define POINT_LIGHT 0
#define DIRECTION_LIGHT 1

#define PHONG_REFLECTION_LIGHT false
#define BLINN_PHONG_REFLECTION_LIGHT true


struct Lamp {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 attenuation;
	vec3 position;
};


struct Sun {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


in VertexData {
	vec4 position;
	vec2 uv;
	vec3 normal;

	vec4 position_light;
} in_data;

// Dane potrzebne do wyliczenia cienia
uniform sampler2D tex_shadowMap;

// Pozycja kamery przekazana z aplikacji
uniform vec3 cameraPos;

// uchwyt tekstury
uniform sampler2D uTexture;
uniform bool hasTex;

uniform int selected = 0;

// model
uniform bool isLight;
uniform vec3 lightModelColor;

uniform bool active_field;

uniform Sun sun;
uniform Lamp lights[NUMBER_OF_LIGHTS];
uniform Material my_material;

// Kolor ostateczny
out vec4 outColor;

// ---------------------------------------------------------------------------
// Zwraca [0-1], gdzie 1 oznacza ze fragment jest calkowicie w cieniu
// ---------------------------------------------------------------------------
float calcDirectionalShadow() {
	if (in_data.position_light.z > 1.0) {
		return 0.0;
	}
	//Brak cienia
	//return 0.0;

	// Korekcja perspektywiczna (dla oswietlenia kierunkowego niepotrzebna)
	vec3 projCoords = in_data.position_light.xyz / in_data.position_light.w;
	// przeksztalcenie wartosci [-1,+1] na [0,1]
	projCoords = projCoords * 0.5 + 0.5;

	// pobranie z tekstury shadowMap odleglosci od zrodla swiatla fragmentu
	// do fragmentu oswietlonego na drodze do aktualnego fragmentu
	float closestDepth = texture(tex_shadowMap, projCoords.xy).r;

	// obliczenie aktualnej odleglosci od zrodla swiatla
	float currentDepth = projCoords.z;

	// Sprawdzenie czy fragment jest w cieniu po odleglosci
	//return (currentDepth > closestDepth) ? 1.0 : 0.0;

	// Shadow acne
	// Parametry i stale ustawiamy eksperymentalnie
	vec3 my_direction = vec3(0.0);
	float bias = max(0.004 * (1.0 - dot(in_data.normal, sun.direction)), 0.001);
	//return (currentDepth - bias > closestDepth) ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(tex_shadowMap, 0);
	for (float x = -1; x <= 1; x +=1.0)
	for (float y = -1; y <= 1; y +=1.0) {
	float pcfDepth = texture(tex_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
	shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	}
	return (shadow / 9.0);
}

vec3 calculate_ambient(vec3 light_ambient, vec3 material_ambient) {
	vec3 ambient_part = light_ambient * material_ambient;

	return ambient_part;
}

vec3 calculate_sun_light_vector(vec3 light_direction) {
	return -light_direction;
}

vec3 calculate_lamp_light_vector(vec3 light_position) {
	return normalize(light_position - in_data.position.xyz);
}

vec3 calculate_diffuse(vec3 light_diffuse, vec3 material_diffuse, vec3 light_vector) {
	float diff = max(dot(in_data.normal, light_vector), 0);
	vec3 diffuse_part = diff *light_diffuse * material_diffuse;

	return diffuse_part;
}

float calculate_phong_reflection_light(float material_shininess, vec3 L, vec3 E) {
	vec3 R = reflect(-E, in_data.normal);
	float reflection = pow(max(dot(R, L), 0), material_shininess);

	return reflection;
}

float calculate_blinn_phong_reflection_light(float material_shininess, vec3 L, vec3 E) {
	vec3 H = normalize(L + E);
	float reflection = pow(max(dot(H, in_data.normal), 0), material_shininess);

	return reflection;
}

vec3 calculate_specular(Material material, vec3 light_specular, vec3 L, bool reflection_type) {
	vec3 E = normalize(cameraPos - in_data.position.xyz);
	float reflection = 1.0;

	if (reflection_type == PHONG_REFLECTION_LIGHT)
		reflection = calculate_phong_reflection_light(material.shininess, L, E);
	else
		reflection = calculate_blinn_phong_reflection_light(material.shininess, L, E);

	vec3 specular_part = reflection * light_specular * material.specular;

	return specular_part;
}

vec3 calculate_sun(Sun light, Material material) {
	vec3 ambient_part = calculate_ambient(light.ambient, material.ambient);

	vec3 L = calculate_sun_light_vector(light.direction);
	vec3 diffuse_part = calculate_diffuse(light.diffuse, material.diffuse, L);

	vec3 specular_part = calculate_specular(material, light.specular, L, PHONG_REFLECTION_LIGHT);

	vec3 light_coef = ambient_part + diffuse_part + specular_part;

	return light_coef;
}

vec3 calculate_lamp(Lamp light, Material material) {
	vec3 ambient_part = calculate_ambient(light.ambient, material.ambient);

	vec3 L = calculate_lamp_light_vector(light.position);
	vec3 diffuse_part = calculate_diffuse(light.diffuse, material.diffuse, L);

	vec3 specular_part = calculate_specular(material, light.specular, L, PHONG_REFLECTION_LIGHT);

	float LV = distance(in_data.position.xyz, light.position);
	float latt = 1 / (light.attenuation.x + light.attenuation.y * LV + light.attenuation.z * LV * LV);
	vec3 light_coef = ambient_part + latt * (diffuse_part + specular_part);

	return light_coef;
}


void main() {
	if (isLight) {
		outColor = vec4(lightModelColor, 1.0);
	}
	else {
		vec3 Color = vec3(0.1);

		if (hasTex) {
			vec4 texColor = texture(uTexture, in_data.uv);
			Color += texColor.xyz;
		}
		else {
			Color += my_material.ambient;
		}

		if (active_field) {
			Color = vec3(0.2, length(in_data.position), 0.2);
		}

		vec3 light_part = vec3(0.0);

		light_part += calculate_sun(sun, my_material);

		for (int i = 0; i < NUMBER_OF_LIGHTS; i++) {
			//light_part += calculate_lamp(lights[i], my_material);
		}

		// Cienie
		float shadowPart = calcDirectionalShadow();

		vec3 fragColor = vec3(0.1, 1.0, 0.1);

		// Obliczanie koloru ostatecznego
		vec3 finalColor = (sun.ambient + (1 - shadowPart) * light_part) * Color;

		outColor = vec4(finalColor, 1.0);

		if (selected == 1) {
			//outColor = vec4(finalColor, 1.0) * vec4(0.2, 0.2, 0.9, 1.0);
			outColor = vec4(0.02, 0.02, 0.35, 1.0);
		}
	}
}

// Fragment shader
#version 330

in vec3 fragColor;
out vec4 outColor;

void main() {
	float d = distance(vec2(1000, 1000), vec2(gl_FragCoord.x, gl_FragCoord.y));

	outColor = vec4(fragColor.r + d / 10000, fragColor.g - 0.9, fragColor.b + 0.2, 1.0);
}

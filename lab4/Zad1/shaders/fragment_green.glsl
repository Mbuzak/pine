// Fragment shader
#version 330

in vec3 fragColor;
out vec4 outColor;

void main() {
	float d = distance(vec2(1000, 1000), vec2(gl_FragCoord.x, gl_FragCoord.y));

	outColor = vec4(fragColor.r - 0.7, fragColor.g + d / 1500, fragColor.b - 1.0, 1.0);
}

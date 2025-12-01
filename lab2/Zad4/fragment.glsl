// Fragment shader
#version 330 core


// Predefiniowane dane wejsciowe
/*
in  vec4 gl_FragCoord;
in  int  gl_PrimitiveID;
*/

// Dane wyjsciowe
out vec4 outColor;


void main() {
    float d = distance(vec2(250, 250), vec2(gl_FragCoord.x, gl_FragCoord.y)) * (sin(1.0)+1.5)*0.0025;
    outColor = mix(vec4(1.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), d);

}

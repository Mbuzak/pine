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
    float x = gl_FragCoord.x / 1000;
    float y = gl_FragCoord.y / 1000;

    float base = float(gl_PrimitiveID / 200.0);

	if (gl_PrimitiveID % 3 == 0)
		outColor = vec4(2 * base, base + x, base + y, 1.0);
	else if ( gl_PrimitiveID % 3 == 1 )
		outColor = vec4(base + x, 2 * base + x, base + y, 1.0);
    else
        outColor = vec4(base + x, base + y, 2 * base, 1.0);

}

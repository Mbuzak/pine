// Fragment shader
#version 330 core


// Predefiniowane dane wejsciowe
/*
in  vec4 gl_FragCoord;
in  int  gl_PrimitiveID;
*/

// Dane wyjsciowe
out vec4 outColor;


void main()
{
    float x = gl_FragCoord.x / 1000.0;
    float y = gl_FragCoord.y / 1000.0;

    float base = float(gl_PrimitiveID / 400.0) + 0.1;

    outColor = vec4(2 * base + x + y, base / 2, base / 2, 1.0);

}

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
    float base = float(gl_PrimitiveID / 400.0) + 0.1;

    outColor = vec4(base, base, base, 1.0);
}

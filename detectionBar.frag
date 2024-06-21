#version 330 core

#define PI 3.14159265359

out vec4 FragColor;
in vec2 texCoords;

uniform float detection;
uniform float anim;
uniform vec3 bgColor;
uniform vec3 barColor;
uniform float aspect;

float sdSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

void main()
{
    vec2 mapUV = texCoords * 2.0 - 1.0;

    mapUV.x *= aspect;

    vec3 col = bgColor;

    vec2 a = vec2(-aspect + 1.0, 0.0);
    vec2 b = vec2(aspect - 1.0, 0.0);

    vec2 c = mix(a, b, detection);

    float k = step(sdSegment(mapUV, a, b), 1.0);

    vec3 barColor2 = mix(barColor, vec3(1.0, 0.0, 0.0), abs(sin(anim)));

    vec3 finBarColor = mix(barColor, barColor2, step(0.5, detection));

    col = mix(col, finBarColor, step(sdSegment(mapUV, a, c), 0.5));

    FragColor = vec4(col, k);

    //FragColor = vec4(mapUV, 0.0, 1.0);
}
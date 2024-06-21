#version 330 core

#define PI 3.14159265359

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D objectTexture;
uniform float aspect;
uniform vec3 objPos;
uniform float objSize;


void main()
{
    vec2 mapUV = texCoords * 0.5 + 0.5;
    mapUV = texCoords;
    mapUV.x *= aspect;

    vec2 objUV = vec2(atan(-objPos.z, -objPos.x) / PI / 2 + 0.5, acos(-objPos.y) / PI);
    objUV.x *= aspect;

    FragColor = vec4(0.0);
    vec2 uv = (mapUV - objUV + vec2(objSize / 2.0)) / objSize;

    uv = min(uv, 1.0);
    uv = max(uv, 0.0);
    if (length(mapUV - objUV) <= 0.1)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    FragColor = texture(objectTexture, uv);
}


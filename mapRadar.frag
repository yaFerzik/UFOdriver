#version 330 core

#define PI 3.14159265359

out vec4 FragColor;
in vec2 texCoords;

uniform vec3 radarPos;
uniform vec3 radarColor;

uniform float radarR;
uniform float planetR;
uniform float flightHeight;


void main()
{
    vec2 mapUV = texCoords;

    float theta = mapUV.y * PI;
    float phi = mapUV.x * PI * 2.0;

    vec3 pos = vec3(sin(theta) * cos(phi), -cos(theta), sin(theta) * sin(phi));

    pos = pos * (planetR + flightHeight);

    float k = step(length((pos - radarPos)), radarR) * 0.5;

    FragColor = vec4(radarColor, k);
}


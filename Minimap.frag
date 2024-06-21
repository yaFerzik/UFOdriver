#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D mapTexture;

float near = 0.01;
float far = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main()
{
    FragColor = vec4(texture(mapTexture, texCoords).xyz, 1.0f);
}
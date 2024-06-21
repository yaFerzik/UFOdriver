#version 330 core
out vec4 FragColor;


in vec3 pos;
in vec3 norm;
in vec3 Color;
in vec2 UV;

in vec3 originalPos;

uniform sampler2D depthTex;

uniform sampler2D diffuse0;

uniform vec3 lightDir;

uniform vec3 beamColor;
uniform vec3 camPos;

float near = 0.01;
float far = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	vec2 uv = pos.xy * 0.5 + 0.5;
	float depth = LinearizeDepth(texture(depthTex, uv).r); 

	if (length(originalPos - camPos) > depth)
		FragColor = vec4(0.0);
	else
	{
		vec3 dirToCam = normalize(originalPos - camPos);

		vec3 normCam = normalize(vec3(dirToCam.x, 0.0, dirToCam.z));

		float k = max(0.0, dot(normCam, norm));

		k = pow(k, 2.0) * 0.5;

		vec3 col = beamColor;

		FragColor = vec4(col, k);
	}
}


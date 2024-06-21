#include "vec.h"
#include <cmath>

float length(vec3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float length(vec4 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}


vec3 normalize(vec3 v)
{
	float l = length(v);

	return vec3{ v.x / l, v.y / l, v.z / l };
}

float dot(vec3 a, vec3 b)
{
	//return vec3{ a.x * b.x, a.y * b.y, a.z * b.z};
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(vec3 a, vec3 b)
{
	return vec3{ a.y * b.z - a.z * b.y,
				 a.z * b.x - a.x * b.z,
				 a.x * b.y - a.y * b.x };
}

vec3 mult(vec3 v, float s)
{
	return vec3{ v.x * s, v.y * s, v.z * s };
}

vec3 add(vec3 a, vec3 b)
{
	return vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

vec4 quatMult(vec4 a, vec4 b) {
	return {
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,  // i
		a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,  // j
		a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,   // k
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z  // 1
	};
}

vec3 quatRotate(vec3 v, vec4 q)
{
	vec3 u(q.x, q.y, q.z);

	float s = q.w;

	return add(add(mult(u, dot(u, v) * 2.0f),
		mult(v, s * s - dot(u, u))),
		mult(cross(u, v), 2.0f * s));
}

vec4 quatFromVectors(vec3 v1, vec3 v2)
{
	v1 = normalize(v1);
	v2 = normalize(v2);

	vec3 u = normalize(cross(v1, v2));

	float theta = acos(dot(v1, v2));

	return vec4(sin(theta / 2) * u.x,
				sin(theta / 2) * u.y,
				sin(theta / 2) * u.z,
				cos(theta / 2));
}

vec4 inverse(vec4 q)
{
	float k = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

	return vec4(-q.x / k, -q.y / k, -q.z / k, q.w / k);
}

vec3 mix(vec3 v1, vec3 v2, float k)
{
	return vec3(v1.x * (1.0 - k) + v2.x * k,
				v1.y * (1.0 - k) + v2.y * k,
				v1.z * (1.0 - k) + v2.z * k);
}
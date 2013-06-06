/******************************************************************************
 *
 * Oak game engine
 * Copyright (c) 2013 Remi Papillie
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 *****************************************************************************/

precision highp float;

uniform float time;
uniform vec3 color;

varying vec2 vertexPosition;

const float INFINITY = 10000.0;
vec3 lightDir = normalize(vec3(1.0, 0.7, -0.6));

vec3 spherePos;

float plane(vec3 pos, vec3 dir)
{
	float s = sign(dir.y);
	vec2 sel = clamp(vec2(s, -s), 0.0, 1.0);
	return dot(sel, vec2(INFINITY, (-pos.y / dir.y)));
}

float sphere(vec3 pos, vec3 dir, float radius)
{
	float b = dot(2.0 * dir, pos);
	float c = dot(pos, pos) - radius * radius;
	float delta = b * b - 4.0 * c;
	if (delta < 0.0)
		return INFINITY;
	
	float t = (-b - sqrt(delta)) * 0.5;
	
	float s = sign(t);
	vec2 sel = clamp(vec2(s, -s), 0.0, 1.0);
	return dot(sel, vec2(t, INFINITY));
}

vec4 raytrace(vec3 pos, vec3 dir)
{
	vec3 localPos = pos - spherePos;
	float obj1 = plane(pos, dir);
	float obj2 = sphere(localPos, dir, 10.0);
	
	if (obj1 < obj2)
	{
		return vec4(0.0, 1.0, 0.0, obj1);
	}
	else
	{
		vec3 point = localPos + dir * obj2;
		return vec4(normalize(point), obj2);
	}
}

float raytraceDistanceOnly(vec3 pos, vec3 dir)
{
	vec3 localPos = pos - spherePos;
	float obj1 = plane(pos, dir);
	float obj2 = sphere(localPos, dir, 10.0);
	return min(obj1, obj2);
}

vec3 calcDiffuse(vec3 pos)
{
	return vec3(fract(pos.z * 0.1) + fract(pos.x * 0.1 + time));
}

void main(void)
{
	vec3 pos = vec3(0.0, 12.0, 0.0);
	vec2 uv = vertexPosition;
	vec3 dir = normalize(vec3(uv.x * 16.0 / 9.0, uv.y, 1.0));
	
	vec3 skyColor = mix(vec3(0.4, 0.5, 0.6), vec3(0.7, 0.9, 1.0), uv.y * 0.5 + 0.5);
	spherePos = vec3(sin(time) * 10.0, 0.0, 25.0);
	
	vec4 intersection = raytrace(pos, dir);
	pos = pos + intersection.w * dir;
	
	vec3 outColor = calcDiffuse(pos);
	float extinction = exp(-abs(pos.z) * 0.02);
	
	if (intersection.w < INFINITY)
	{
		// shadow ray
		float shadow = float(raytraceDistanceOnly(pos + lightDir * 0.2, lightDir) == INFINITY) * 0.8 + 0.2;
		
		// direct light
		float light = clamp(dot(intersection.xyz, lightDir), 0.0, 1.0) * shadow;
		outColor *= light;
		
		// secondary ray
		dir = reflect(intersection.xyz, dir);
		float dist = raytraceDistanceOnly(pos + dir * 0.1, dir);
		pos = pos + dist * dir;
		vec3 diffuse = calcDiffuse(pos);
		float fog = exp(-abs(pos.z) * 0.02);
		outColor += (diffuse * fog + vec3(1.0 - fog)) * 0.3;
	}
	
	outColor = outColor * extinction + skyColor * (1.0 - extinction);
	
	outColor = sqrt(outColor); // gamma
	outColor *= vec3(1.0 - pow(length(uv), 2.0) * 0.5); // vignette
	
	gl_FragColor = vec4(outColor, 1.0);
}

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

varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec2 fragUV;
varying float viewDepth;

vec3 lightDir = normalize(vec3(1.0, 0.7, -0.6));
vec3 lightPos = vec3(4.0, 2.0, 0.0);

void main(void)
{
	vec3 normal = normalize(fragNormal);
	
	// directional light
	float light = clamp(dot(normal, lightDir), 0.0, 1.0) * 0.1;
	
	// point light
	vec3 dir = lightPos - fragPosition;
	float dirLength = length(dir);
	light += clamp(dot(normal, dir) / dirLength, 0.0, 1.0) / (0.1 * dirLength * dirLength);
	
	// simple procedural texture
	float f = fract(fragUV.x * 3.0) * fract(fragUV.y * 3.0);
	float style = pow(f, 2.0);
	float border = pow(clamp((abs(f - 0.5) - 0.4) * 10.0, 0.0, 1.0), 2.0);
	vec3 diffuse = style * vec3(0.8, 1.0, 0.8) + border * vec3(1.0, 1.0, 0.7);
	
	vec3 outColor = vec3(light) * diffuse;
	
	// fog
	float fog = 1.0 - exp(-viewDepth * 0.02);
	outColor = mix(outColor, vec3(0.6, 0.8, 0.9), fog);
	
	//outColor = sqrt(outColor); // gamma
	gl_FragColor = vec4(outColor, 1.0);
}

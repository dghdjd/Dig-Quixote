#version 330

// From Vertex Shader
in vec3 vcolor;

// Application data
uniform vec3 fcolor;
uniform float time;
uniform int light_up;
uniform vec2 p_pos;
// Output color
layout(location = 0) out vec4 color;

void main()
{
	vec3 color1 = vec3(1.0, 0.0, 0.0); // Red
    vec3 color2 = vec3(1.0, 0.5, 0.0); // Light Yellow
	float radius = 7.f;
	if (light_up == 1)
	{
		float t = (sin(time) * 0.5 + 0.5);
		t = pow(t, 3.0);
		vec3 finalColor = mix(color1, color2, t);

		color = vec4(finalColor * vcolor, 0.5);
	}
	else 
	{
		color = vec4(fcolor * vcolor, 1.0);
	}
	
}
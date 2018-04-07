R"zzz(#version 410 core

// flat in vec4 normal;

// in vec4 light_direction;
out vec4 fragment_color;
void main()
{
	// vec4 color = abs(normal) + vec4(0.0, 0.0, 0.0, 1.0);
	// float dot_nl = dot(normalize(light_direction), normal);
	// dot_nl = clamp(dot_nl, 0.0, 1.0);
	// fragment_color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
}
)zzz"
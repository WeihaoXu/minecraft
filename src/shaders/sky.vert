R"zzz(#version 410 core
in vec4 vertex_position;
uniform vec3 sky_offset;
uniform float day_time;
out float vs_day_time;


void main()
{
	gl_Position = vertex_position + vec4(sky_offset, 0.0);
	vs_day_time = day_time;
}
)zzz"
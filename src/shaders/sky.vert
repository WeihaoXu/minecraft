R"zzz(#version 410 core
in vec4 vertex_position;
uniform vec3 sky_offset;


void main()
{
	gl_Position = vertex_position + vec4(sky_offset, 1.0);
}
)zzz"
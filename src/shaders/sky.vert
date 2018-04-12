R"zzz(#version 410 core
in vec4 vertex_position;
uniform vec3 sky_offset;

in vec2 uv;
out vec2 vs_uv;


void main()
{
	gl_Position = vertex_position + vec4(sky_offset, 0.0);
	vs_uv = uv;
}
)zzz"
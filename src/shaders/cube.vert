R"zzz(#version 410 core
in vec4 vertex_position;
in vec4 normal;
in vec2 uv;
in vec3 offset;

// out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
// uniform vec4 light_position;
uniform vec3 camera_position;


void main()
{
	// gl_Position = vertex_position + vec4(offsets[gl_InstanceID ], 0.0);	
	gl_Position = vertex_position + vec4(offset, 0.0);	
	// vs_light_direction = -gl_Position + light_position;
	vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	vs_normal = normal;
	vs_uv = uv;
}
)zzz"
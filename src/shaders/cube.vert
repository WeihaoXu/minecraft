R"zzz(#version 410 core
in vec4 vertex_position;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position;
uniform vec3 offsets[128];

// out vec4 vs_light_direction_0;
// out vec4 vertex_position_world_0;
// out vec4 vs_light_direction;
// out vec4 vertex_position_world;
void main()
{
	vec4 pos = vec4(vertex_position.xyz + offsets[gl_InstanceID], 1.0);
	mat4 mvp = projection * view * model;
	gl_Position = mvp * pos;
	
	// gl_Position = view * vertex_position;
	// vs_light_direction_0 = -gl_Position + view * light_position;
	// vertex_position_world_0 = vertex_position;
	// vs_light_direction = -gl_Position + view * light_position;
	// vertex_position_world = vertex_position;
}
)zzz"
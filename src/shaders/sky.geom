R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
in vec2 vs_uv[];

out vec2 uv_coords;
out vec4 world_position;


void main() {
	int n = 0;
	for (n = 0; n < gl_in.length(); n++) {
		world_position = gl_in[n].gl_Position;
		gl_Position = projection * view * model * gl_in[n].gl_Position;
		uv_coords = vs_uv[n];
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"

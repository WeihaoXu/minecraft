R"zzz(#version 410 core

in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 camera_direction;
in vec2 uv_coords;
// uniform vec4 diffuse;
// uniform vec4 ambient;
// uniform vec4 specular;
// uniform float shininess;
// uniform float alpha;
// uniform sampler2D textureSampler;
out vec4 fragment_color;

void main()
{

	vec4 diffuse = vec4(0.0, 1.0, 0.0, 0.0);
	vec4 ambient = vec4(0.5, 0.5, 0.5, 0.0);
	vec4 specular = vec4(0.2, 0.2, 0.2, 0.0);
	float shininess = 0.5;

	vec3 color = vec3(diffuse);
	float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	vec4 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);
	color = clamp(dot_nl * color + vec3(ambient) + vec3(spec), 0.0, 1.0);
	fragment_color = vec4(color, 1.0);

	/*
	vec3 base_color = vec3(0.0, 1.0, 0.0);
	float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	vec3 color = clamp(dot_nl * base_color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	*/

}

)zzz"
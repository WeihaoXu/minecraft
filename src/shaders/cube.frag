R"zzz(#version 410 core

in float cube_type;
in vec4 face_normal;
in vec4 vertex_normal;
// in vec4 light_direction;
in vec4 camera_direction;
in vec2 uv_coords;
in vec4 world_position;
// uniform vec4 diffuse;
// uniform vec4 ambient;
// uniform vec4 specular;
// uniform float shininess;
// uniform float alpha;

uniform sampler2D perm_texture;

out vec4 fragment_color;


#define ONE 0.00390625
#define ONEHALF 0.001953125
#define eps 0.001
#define TYPE_WATER 0.0
#define TYPE_DIRT_GRASS 1.0
#define TYPE_DIRT 2.0
#define TYPE_STONE 3.0

vec4 light_direction = vec4(normalize(vec3(0.4, 1.0, 0.6)), 0.0);

float lerp(float a, float b, float t) { 
  return a + t * (b - a); 
}

vec2 lerp2D(vec2 a, vec2 b, float t) { 
  return a + t * (b - a); 
}

vec3 lerp3D(vec3 a, vec3 b, float t) { 
  return a + t * (b - a); 
}

vec4 lerp4D(vec4 a, vec4 b, float t) { 
  return a + t * (b - a); 
}

float fade(float t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade
}
 
// I followed this idea to pass in gradient vectors to GPU as textures: 
// http://www.sci.utah.edu/~leenak/IndStudy_reportfall/MarbleCode.txt
float perlinNoise(vec3 P)
{

  vec3 Pi = ONE*floor(P)+ONEHALF;   
  vec3 Pf = P-floor(P);
  
  // Noise contributions from (x=0, y=0), z=0 and z=1
  float perm00 = texture(perm_texture, Pi.xy).a ;
  vec3  grad000 = texture(perm_texture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
  float n000 = dot(grad000, Pf);
  vec3  grad001 = texture(perm_texture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n001 = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

  // Noise contributions from (x=0, y=1), z=0 and z=1
  float perm01 = texture(perm_texture, Pi.xy + vec2(0.0, ONE)).a ;
  vec3  grad010 = texture(perm_texture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
  float n010 = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
  vec3  grad011 = texture(perm_texture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n011 = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

  // Noise contributions from (x=1, y=0), z=0 and z=1
  float perm10 = texture(perm_texture, Pi.xy + vec2(ONE, 0.0)).a ;
  vec3  grad100 = texture(perm_texture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
  float n100 = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
  vec3  grad101 = texture(perm_texture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n101 = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

  // Noise contributions from (x=1, y=1), z=0 and z=1
  float perm11 = texture(perm_texture, Pi.xy + vec2(ONE, ONE)).a ;
  vec3  grad110 = texture(perm_texture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
  float n110 = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
  vec3  grad111 = texture(perm_texture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n111 = dot(grad111, Pf - vec3(1.0, 1.0, 1.0));

  // Blend contributions along x
  vec4 n_x = lerp4D(vec4(n000, n001, n010, n011), vec4(n100, n101, n110, n111), fade(Pf.x));

  // Blend contributions along y
  vec2 n_xy = lerp2D(n_x.xy, n_x.zw, fade(Pf.y));

  // Blend contributions along z
  float n_xyz = lerp(n_xy.x, n_xy.y, fade(Pf.z));
 
  return n_xyz;
}


float turbulence(int octaves, vec3 P, float lacunarity, float gain)
{ 
  float sum = 0;
  float scale = 1;
  float totalgain = 1;
  for(int i=0;i<octaves;i++){
    sum += totalgain*perlinNoise(P*scale);
    scale *= lacunarity;
    totalgain *= gain;
  }
  return abs(sum);
}

vec4 generateGrassColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
  float shininess = 0.1;
  vec3 specular = vec3(0.1, 0.1, 0.1) * 0;


  float freq_world = 0.5721;
  

  vec3 color_0 = vec3(128, 255, 0) / 255.0 * 1.0;
 
  vec3 color_1 = vec3(128, 255, 0) / 255.0 * 0.5;

  float perlin_noise = cos(turbulence(5, world_pos.xyz, 3, 0.9) * freq_world);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
  dot_nl = clamp(dot_nl, 0.0, 1.0);
  
  vec3 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);

  vec3 color = clamp(dot_nl * diffuse + ambient + spec, 0.0, 1.0);
  return vec4(color, 1.0);
}


vec4 generateDirtColor(vec4 world_pos) {
  vec3 ambient = vec3(222, 122, 63) / 255.0 * 0.2;
  float shininess = 0.1;
  vec3 specular = vec3(0.1, 0.1, 0.1) * 0;

  float freq_world = 7.721;

  vec3 color_0 = vec3(128,0,0) / 255.0;
  vec3 color_1 = vec3(0,0,0) / 255.0; // black

  float perlin_noise = perlinNoise(world_pos.xyz * freq_world);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
  dot_nl = clamp(dot_nl, 0.0, 1.0);
  
  vec3 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);

  vec3 color = clamp(dot_nl * diffuse + ambient + spec, 0.0, 1.0);
  return vec4(color, 1.0);
}


vec4 generateWaterColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
  float shininess = 0.1;
  vec3 specular = vec3(64, 164, 223) * 0.0;

  float freq_world = 7.721;

  vec3 color_0 = vec3(64, 164, 223) / 255.0 * 1.0;
  vec3 color_1 = vec3(64, 164, 223) / 255.0 * 0.1;

  float perlin_noise = perlinNoise(world_pos.xyz * freq_world);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
  dot_nl = clamp(dot_nl, 0.0, 1.0);
  
  vec3 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);

  vec3 color = clamp(dot_nl * diffuse + ambient + spec, 0.0, 0.5);
  return vec4(color, 1.0);
}

vec4 generateStoneColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
  float shininess = 0.1;
  vec3 specular = vec3(64, 164, 223) * 0.0;

  float freq_world = 7.721;

  vec3 color_0 = vec3(71, 79, 100) / 255.0 * 1.0;
  vec3 color_1 = vec3(71, 79, 100) / 255.0 * 0.1;

  float perlin_noise = perlinNoise(world_pos.xyz * freq_world);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
  dot_nl = clamp(dot_nl, 0.0, 1.0);
  
  vec3 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);

  vec3 color = clamp(dot_nl * diffuse + ambient + spec, 0.0, 0.5);
  return vec4(color, 1.0);
}

void main()
{

  if(cube_type < TYPE_WATER + eps) {
    if(vertex_normal.y > 0.001) {
      fragment_color = generateWaterColor(world_position);
    }
    else {
      fragment_color = vec4(0.0, 0.0, 0.0, 1.0);
    } 
  }
  else if(cube_type < TYPE_DIRT_GRASS + eps) {
    if(vertex_normal.y > 0.001) {
      fragment_color = generateGrassColor(world_position);
    }
    else {
      fragment_color = generateDirtColor(world_position);
    } 
  }
  else if(cube_type < TYPE_DIRT + eps) {
    fragment_color = vec4(generateDirtColor(world_position).xyz * 0.5, 1.0);
  }
  else if(cube_type < TYPE_STONE + eps) {
    fragment_color = generateStoneColor(world_position);
  }

  

}

)zzz"
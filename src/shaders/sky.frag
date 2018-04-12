R"zzz(#version 410 core


in float day_time;
in vec4 world_position;
uniform sampler2D perm_texture;

out vec4 fragment_color;


#define ONE 0.00390625
#define ONEHALF 0.001953125
#define eps 0.001
#define TYPE_DAY 0.0
#define TYPE_DUSK 1.0
#define TYPE_NIGHT 2.0
#define TYPE_DAWN 3.0

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
  //return t*t*(3.0-2.0*t); // Old fade
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

float turbulence(vec3 P, float size)
{
  float noise = 0.0;
  float init_size = size;

  while(size >= 1) {
    noise += perlinNoise(P / size) * size;
    size /= 2.0;
  }
  noise /= init_size;
  return noise;
}



vec4 generateDayColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
 
  float freq_world = 7.721;

  vec3 color_0 = vec3(255,255,220) / 255.0 * 1.0;  //vec3(135, 206, 250) / 255.0 * 1.0;   
  vec3 color_1 = vec3(255, 255, 255) / 255.0 * 1.0; 

  float perlin_noise = turbulence(world_pos.xyz, 30.0);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  vec3 color = clamp(diffuse, 0.0, 1.0);

  return vec4(color, 1.0);
}

vec4 generateDuskColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
 
  float freq_world = 7.721;

  vec3 color_0 = vec3(255, 60, 60) / 255.0 * 1.0; //vec3(23,2,102) / 255.0 * 1.0;
  vec3 color_1 = vec3(96,96,96) / 255.0 * 1.0;

  float perlin_noise = turbulence(world_pos.xyz, 30.0);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  vec3 color = clamp(diffuse, 0.0, 1.0);

  return vec4(color, 1.0);
}

vec4 generateNightColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
 
  float freq_world = 7.721;

  vec3 color_0 = vec3(0,0,0) / 255.0 * 1.0; //vec3(24,15,42) / 255.0 * 1.0;
  vec3 color_1 = vec3(24,24,24) / 255.0 * 1.0;

  float perlin_noise = turbulence(world_pos.xyz, 30.0);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  vec3 color = clamp(diffuse, 0.0, 1.0);

  return vec4(color, 1.0);
}

vec4 generateDawnColor(vec4 world_pos) {
  vec3 ambient = vec3(0.1, 0.1, 0.1) * 0;
 
  float freq_world = 7.721;

  vec3 color_0 = vec3(23,2,102) / 255.0 * 1.0;
  vec3 color_1 = vec3(96,96,96) / 255.0 * 1.0;

  float perlin_noise = turbulence(world_pos.xyz, 30.0);

  vec3 diffuse = lerp3D(color_0, color_1, perlin_noise);
  vec3 color = clamp(diffuse, 0.0, 1.0);

  return vec4(color, 1.0);
}

void main()
{
   if(day_time < TYPE_DAY + eps) {
    fragment_color = generateDayColor(world_position);
   }
   else if(day_time < TYPE_DUSK + eps) {
    fragment_color = generateDuskColor(world_position);
   }
   else if (day_time < TYPE_NIGHT + eps){
    fragment_color = generateNightColor(world_position);
   }
   else if (day_time < TYPE_DAWN + eps){
    fragment_color = generateDawnColor(world_position);
   } else {
    fragment_color = generateDayColor(world_position);
   }
}

)zzz"
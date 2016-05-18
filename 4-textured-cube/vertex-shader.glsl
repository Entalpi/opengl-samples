#version 330 core

in vec3 position;
in vec4 vColor;

out vec4 fColor; // This name must match the name in the fragment shader in order to work

uniform mat3 transform_x; // the transformation matrix for x
uniform mat3 transform_y; // the transformation matrix for y
uniform mat3 transform_z; // the transformation matrix for z

in vec2 vTexcoord;   // passthrough shading for interpolated textures
out vec2 fTexcoord;

void main() {
  gl_Position = vec4(transform_z * transform_y * transform_x * position, 1.0f);
  fColor = vColor;
  fTexcoord = vTexcoord;
}

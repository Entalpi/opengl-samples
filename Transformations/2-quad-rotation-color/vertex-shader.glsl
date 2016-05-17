#version 330 core

in vec3 position;
in vec4 vColor;

out vec4 fColor; // This name must match the name in the fragment shader in order to work

uniform mat3 trans; // the transformation matrix

void main() {
  gl_Position = vec4(trans * position, 1.0f);
  fColor = vColor;
}

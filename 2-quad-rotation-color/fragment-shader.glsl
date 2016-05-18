#version 330 core

in vec4 fColor; // This name must match the name in the vertex shader in order to work
out vec4 outColor;

void main() {
  outColor = fColor;
  // outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <stdlib.h> // calloc, free
#include <stdbool.h>

bool load_shader_source(char *filename, const GLchar ***shader_source) {
  **shader_source = "";
  printf("%s\n", filename);
  return true;
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Window *window =
      SDL_CreateWindow("OpenGL with SDL2", 0, 0, 500, 500,
                       SDL_WINDOW_OPENGL | SDL_WINDOWPOS_CENTERED);

  SDL_GLContext *context = SDL_GL_CreateContext(window);

  glewExperimental = true;
  glewInit();

  GLfloat vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

  GLuint VBO;            // VBO - vertex buffer object
  glGenBuffers(1, &VBO); // generate a 'name for the object'

  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind/make VBO the active object

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices,
               GL_STATIC_DRAW); // upload VBO to OpenGL

  const GLchar *vertexShaderSource[] = {
      "#version 330 core in vec3 position; void main() { gl_Position = "
      "vec4(position, 1.0f);}"};
  // load_shader_source("vertex-shader.glsl", &vertexShaderSource);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  GLint vertexShaderStatus;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);

  const GLchar *fragmentShaderSource[] = {
      "#version 330 core out vec4 outColor; void "
      "main() { outColor = vec4(1.0f, "
      "1.0f, 1.0f, 1.0f); }"};
  // load_shader_source("fragment-shader.glsl", &fragmentShaderSource);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  GLint fragmentShaderStatus;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);

  printf("Fragment: %i, Vertex: %i\n", fragmentShaderStatus,
         vertexShaderStatus);
  char buffer[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
  printf("%s\n", buffer);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Bind Vertex Array Object - OpenGL core REQUIRES a VERTEX ARRAY OBJECT in
  // order to render ANYTHING.
  // An object that stores vertex array bindings
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // make the VAO the active one ..

  // Then set our vertex attributes pointers, only doable AFTER linking
  GLuint positionAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(positionAttrib);

  bool DONE = false;

  while (!DONE) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        DONE = true;
      }
    }

    // Draw the object
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  return 0;
}

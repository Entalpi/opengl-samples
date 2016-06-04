#include "main.h"

Vec3 camera_move_forward(Camera cam) {
  Vec3 movement = vec_scalar_multiplication(cam.direction, cam.movement_speed);
  return vec_addition(cam.position, movement);
}

Vec3 camera_move_backward(Camera cam) {
  Vec3 movement = vec_scalar_multiplication(cam.direction, cam.movement_speed);
  return vec_subtraction(cam.position, movement);
}

Vec3 camera_move_right(Camera cam) {
  Vec3 movement = normalize(cross(cam.direction, cam.up));
  return vec_subtraction(
      cam.position, vec_scalar_multiplication(movement, cam.movement_speed));
}

Vec3 camera_move_left(Camera cam) {
  Vec3 movement = normalize(cross(cam.direction, cam.up));
  return vec_addition(cam.position,
                      vec_scalar_multiplication(movement, cam.movement_speed));
}

Vec3 camera_direction(Camera cam) {
  float rad = M_PI / 180;
  float pitch = cam.pitch;
  Vec3 result;
  result.x = -sin(cam.yaw * rad) * cos(pitch * rad);
  result.y = sin(pitch * rad);
  result.z = -cos(cam.yaw * rad) * cos(pitch * rad);

  printf("Pitch: %.2f Yaw: %.2f ", pitch, cam.yaw);
  printf("Camera position: x:%.2f y:%.2f z:%.2f ", cam.position.x,
         cam.position.y, cam.position.z);
  printf(", direction: x:%.2f y:%.2f z:%.2f\n", result.x, result.y, result.z);
  return normalize(result);
}

// Based on GLM's lookAt function, returns a 4x4 matrix of GLfloats
/// @param eye Position of the camera
/// @param center Position where the camera is looking at
/// @param up Normalized up vector, how the camera is oriented,Typically (0, 0,
/// 1) ROW MAJOR
GLfloat *lookAt(Vec3 eye, Vec3 center, Vec3 up) { // FIXME: Does not quite work
  Vec3 f = normalize(vec_subtraction(eye, center)); // 'forward'
  Vec3 s = normalize(cross(f, up));                 // 'right'
  Vec3 u = cross(s, f);                             // 'up'
  GLfloat tempMatrix[4][4];
  tempMatrix[0][0] = s.x;
  tempMatrix[1][0] = s.y;
  tempMatrix[2][0] = s.z;
  tempMatrix[3][0] = 0.0f;
  tempMatrix[0][1] = u.x;
  tempMatrix[1][1] = u.y;
  tempMatrix[2][1] = u.z;
  tempMatrix[3][1] = 0.0f;
  tempMatrix[0][2] = -f.x;
  tempMatrix[1][2] = -f.y;
  tempMatrix[2][2] = -f.z;
  tempMatrix[3][2] = 0.0f;
  tempMatrix[0][3] = -dot(s, eye);
  tempMatrix[1][3] = -dot(u, eye);
  tempMatrix[2][3] = -dot(f, eye); // GLM says no minus , other's say minus ...
  tempMatrix[3][3] = 1.0f;
  GLfloat *matrix = calloc(1, sizeof(tempMatrix));
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

// Camera combined rotation matrix (y, x) & translation matrix
GLfloat *FPSViewRH(Vec3 eye, float pitch, float yaw) {
  float rad = M_PI / 180;
  float cosPitch = cos(pitch * rad);
  float sinPitch = sin(pitch * rad);
  float cosYaw = cos(yaw * rad);
  float sinYaw = sin(yaw * rad);
  Vec3 xaxis = {cosYaw, 0, -sinYaw};
  Vec3 yaxis = {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
  Vec3 zaxis = {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};
  GLfloat tempMatrix[4][4];
  tempMatrix[0][0] = xaxis.x;
  tempMatrix[0][1] = yaxis.x;
  tempMatrix[0][2] = zaxis.x;
  tempMatrix[0][3] = 0.0f;
  tempMatrix[1][0] = xaxis.y;
  tempMatrix[1][1] = yaxis.y;
  tempMatrix[1][2] = zaxis.y;
  tempMatrix[1][3] = 0.0f;
  tempMatrix[2][0] = xaxis.z;
  tempMatrix[2][1] = yaxis.z;
  tempMatrix[2][2] = zaxis.z;
  tempMatrix[2][3] = 0.0f;
  tempMatrix[3][0] = -dot(xaxis, eye);
  tempMatrix[3][1] = -dot(yaxis, eye);
  tempMatrix[3][2] = -dot(zaxis, eye); // GLM says no minus , other's say minus
  tempMatrix[3][3] = 1.0f;
  GLfloat *matrix = calloc(1, sizeof(tempMatrix));
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

GLfloat *perspective_matrix(float z_near, float z_far, float fov) {
  float rad = M_PI / 180;
  float tanHalf = tan(fov * rad / 2);
  float a = 1 / tanHalf;
  float b = 1 / tanHalf;
  float c = -(z_far + z_near) / (z_far - z_near);
  float d = -(2 * z_far * z_near) / (z_far - z_near);
  GLfloat tempMatrix[4][4] = {{a, 0.0f, 0.0f, 0.0f},
                              {0.0f, b, 0.0f, 0.0f},
                              {0.0f, 0.0f, c, d},
                              {0.0f, 0.0f, -1.0f, 0.0f}};
  GLfloat *matrix = calloc(1, sizeof(tempMatrix));
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

GLfloat *scaling_matrix(float scale) {
  GLfloat tempMatrix[4][4] = {{scale, 0.0f, 0.0f, 0.0f},
                              {0.0f, scale, 0.0f, 0.0f},
                              {0.0f, 0.0f, scale, 0.0f},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
  GLfloat *matrix = calloc(1, sizeof(tempMatrix));
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

GLfloat *translation_matrix(float x, float y, float z) {
  GLfloat tempMatrix[4][4] = {{1.0f, 0.0f, 0.0f, x},
                              {0.0f, 1.0f, 0.0f, y},
                              {0.0f, 0.0f, 1.0f, z},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
  GLfloat *matrix = calloc(1, sizeof(tempMatrix));
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

Cube new_cube(Color4 *colors) {
  Cube cube;
  Point3 a = new_point3(-0.5f, -0.5f, 0.5f);
  Point3 b = new_point3(0.5f, -0.5f, 0.5f);
  Point3 c = new_point3(0.5f, 0.5f, 0.5f);
  Point3 d = new_point3(-0.5f, 0.5f, 0.5f);
  Point3 vertices1[] = {a, b, c, d};
  Point2 tex_a = new_point2(0.0f, 0.0f);
  Point2 tex_b = new_point2(1.0f, 0.0f);
  Point2 tex_c = new_point2(1.0f, 1.0f);
  Point2 tex_d = new_point2(0.0f, 1.0f);
  Point2 texCoords1[] = {tex_a, tex_b, tex_c, tex_d};
  Quad quad1 = new_quad(vertices1, colors, texCoords1);
  cube.quads[0] = quad1;

  Point3 e = new_point3(-0.5f, -0.5f, -0.5f);
  Point3 f = new_point3(0.5f, -0.5f, -0.5f);
  Point3 g = new_point3(0.5f, 0.5f, -0.5f);
  Point3 h = new_point3(-0.5f, 0.5f, -0.5f);
  Point3 vertices2[] = {e, f, g, h};
  Point2 tex_e = new_point2(1.0f, 0.0f);
  Point2 tex_f = new_point2(0.0f, 0.0f);
  Point2 tex_g = new_point2(0.0f, 1.0f);
  Point2 tex_h = new_point2(1.0f, 1.0f);
  Point2 texCoords2[] = {tex_e, tex_f, tex_g, tex_h};
  Quad quad2 = new_quad(vertices2, colors, texCoords2);
  cube.quads[1] = quad2;
  cube.position = new_point3(0.0f, 0.0f, 0.0f);
  return cube;
}

GLfloat *cube_to_floats(Cube *cube) {
  GLfloat *cube_floats = calloc(1, sizeof(Quad) * 6);
  for (size_t i = 0; i < 6; i++) {
    Quad quad = cube->quads[i];
    GLfloat *quad_floats = quad_to_floats(&quad);
    int k = 0;
    for (size_t j = i * sizeof(Quad); j < sizeof(Quad); j++) {
      cube_floats[j] = quad_floats[k];
      k++;
    }
  }
  return cube_floats;
}

Vertex new_vertex(Point3 point, Color4 color, Point2 texCoord) {
  Vertex vertice;
  vertice.color = color;
  vertice.position = point;
  vertice.texCoord = texCoord;
  return vertice;
}

// Input: 4 points, 4 colors
//  d ----- c   <-- Point & color pairs
//  |       |
//  a ----- b   0 = a, 1 = b, 2 = c, 3 = d
Quad new_quad(Point3 *points, Color4 *colors, Point2 *texCoords) {
  Quad quad;
  Point3 a = points[0];
  Point3 b = points[1];
  Point3 c = points[2];
  Point3 d = points[3];
  quad.vertices[0] = new_vertex(a, colors[0], texCoords[0]);
  quad.vertices[1] = new_vertex(b, colors[1], texCoords[1]);
  quad.vertices[2] = new_vertex(c, colors[2], texCoords[2]);
  quad.vertices[3] = new_vertex(d, colors[3], texCoords[3]);
  return quad;
}

// Converts a quad to vertices
// 1 quad = 2 triangles => 4 vertices = 4 points & 4 colors
GLfloat *quad_to_floats(Quad *quad) {
  GLfloat *floats = calloc(1, 4 * sizeof(Vertex));
  int j = 0;
  for (size_t i = 0; i < 4 * 9; i += 9) {
    Vertex vertex = quad->vertices[j];
    floats[i] = vertex.position.x;
    floats[i + 1] = vertex.position.y;
    floats[i + 2] = vertex.position.z;
    floats[i + 3] = vertex.color.r;
    floats[i + 4] = vertex.color.g;
    floats[i + 5] = vertex.color.b;
    floats[i + 6] = vertex.color.a;
    floats[i + 7] = vertex.texCoord.x;
    floats[i + 8] = vertex.texCoord.y;
    j++;
  }
  return floats;
}

// Assumes the file exists and will seg. fault otherwise.
const GLchar *load_shader_source(char *filename) {
  assert(filename != NULL);
  FILE *file = fopen(filename, "r");             // open
  fseek(file, 0L, SEEK_END);                     // find the end
  size_t size = ftell(file);                     // get the size in bytes
  GLchar *shaderSource = calloc(1, size);        // allocate enough bytes
  rewind(file);                                  // go back to file beginning
  fread(shaderSource, size, sizeof(char), file); // read each char into ourblock
  fclose(file);                                  // close the stream
  return shaderSource;
}

// Transformation matrix for the y-plane
GLfloat *transformation_matrix_x(float theta) {
  float x = M_PI / 180;
  GLfloat *matrix = calloc(1, sizeof(GLfloat) * 4 * 4);
  GLfloat tempMatrix[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                              {0.0f, cos(theta * x), -sin(theta * x), 0.0f},
                              {0.0f, sin(theta * x), cos(theta * x), 0.0f},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

GLfloat *transformation_matrix_y(float theta) {
  float x = M_PI / 180;
  GLfloat *matrix = calloc(1, sizeof(GLfloat) * 4 * 4);
  GLfloat tempMatrix[4][4] = {{cos(theta * x), 0.0f, sin(theta * x), 0.0f},
                              {0.0f, 1.0f, 0.0f, 0.0f},
                              {-sin(theta * x), 0.0f, cos(theta * x), 0.0f},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

GLfloat *transformation_matrix_z(float theta) {
  float x = M_PI / 180;
  GLfloat *matrix = calloc(1, sizeof(GLfloat) * 4 * 4);
  GLfloat tempMatrix[4][4] = {{cos(theta * x), -sin(theta * x), 0.0f, 0.0f},
                              {sin(theta * x), cos(theta * x), 0.0f, 0.0f},
                              {0.0f, 0.0f, 1.0f, 0.0f},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
  memcpy(matrix, tempMatrix, sizeof(tempMatrix));
  return matrix;
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Window *window = SDL_CreateWindow("", 0, 0, 750, 750, SDL_WINDOW_OPENGL);
  SDL_GLContext *context = SDL_GL_CreateContext(window);
  atexit(IMG_Quit);

  glewExperimental = true;
  glewInit();

  Color4 colors[] = {new_color_red(), new_color_green(), new_color_blue(),
                     new_color_yellow()};

  size_t numCubes = 10;
  Cube cubes[numCubes];
  for (size_t i = 0; i < numCubes; i++) {
    Cube cube = new_cube(colors);
    cube.position.x = arc4random_uniform(100) % 10;
    cube.position.y = arc4random_uniform(100) % 10;
    cube.position.z -= arc4random_uniform(100) % 10;
    cubes[i] = cube;
    printf("x: %f y: %f z: %f\n", cube.position.x, cube.position.y,
           cube.position.z);
  }

  GLuint VBO;                         // VBO - vertex buffer object
  glGenBuffers(1, &VBO);              // generate a 'name for the object'
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind/make VBO the active object
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubes), NULL,
               GL_STATIC_DRAW); // reserve a large buffer for cube quads
  // Upload only one cube's quads since the model is the same for all cubes
  for (size_t i = 0; i < 2; i++) {
    Cube cube = cubes[0];
    Quad quad = cube.quads[i];
    GLfloat *quad_data = quad_to_floats(&quad);
    glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(Quad), sizeof(Quad), quad_data);
  }

  const GLchar *vertexShaderSource = load_shader_source("vertex-shader.glsl");
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  GLint vertexShaderStatus;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);

  const GLchar *fragmentShaderSource =
      load_shader_source("fragment-shader.glsl");
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

  printf("Position offset: %lu Color offset: %lu\n", offsetof(Vertex, position),
         offsetof(Vertex, color));
  // Then set our vertex attributes pointers, only doable AFTER linking
  GLuint positionAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(positionAttrib);

  GLuint colorAttrib = glGetAttribLocation(shaderProgram, "vColor");
  glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(colorAttrib);

  GLuint texAttrib = glGetAttribLocation(shaderProgram, "vTexcoord");
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, texCoord));
  glEnableVertexAttribArray(texAttrib);

  GLuint transform_x = glGetUniformLocation(shaderProgram, "transform_x");
  GLuint transform_y = glGetUniformLocation(shaderProgram, "transform_y");
  GLuint transform_z = glGetUniformLocation(shaderProgram, "transform_z");
  printf("Trans Mats.: %u %u %u\n", transform_x, transform_y, transform_z);
  GLfloat *testMat = transformation_matrix_x(90.0f);
  for (size_t i = 0; i < 9; i++) {
    printf("%f ", testMat[i]);
    if (i == 2 || i == 5 || i == 8) {
      printf("\n");
    }
  }

  // Model
  GLuint transform_translation =
      glGetUniformLocation(shaderProgram, "transform_translation");
  GLuint transform_scaling =
      glGetUniformLocation(shaderProgram, "transform_scaling");

  // View / camera space
  GLuint camera_view = glGetUniformLocation(shaderProgram, "camera_view");

  // Projection
  GLuint transform_perspective =
      glGetUniformLocation(shaderProgram, "transform_perspective");
  GLfloat *transMat_perspective = perspective_matrix(1, -20, 60);
  glUniformMatrix4fv(transform_perspective, 1, GL_FALSE, transMat_perspective);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // tex.coords (s, t, r) == (x, y, z)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  IMG_Init(IMG_INIT_PNG); // init sdl2_image
  SDL_Surface *image = IMG_Load("retro.png");
  int width = image->w;
  int height = image->h;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image->pixels);
  // https://wiki.libsdl.org/SDL_PixelFormatEnum#type
  printf("W: %i  H: %i Format: %u\n", width, height,
         SDL_PIXELTYPE(image->format->format));

  GLuint indices[] = {// front
                      0, 1, 2, 2, 3, 0,
                      // right
                      1, 5, 6, 6, 2, 1,
                      // back
                      7, 6, 5, 5, 4, 7,
                      // left
                      4, 0, 3, 3, 7, 4,
                      // bot
                      4, 5, 1, 1, 0, 4,
                      // top
                      3, 2, 6, 6, 7, 3};

  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  bool DONE = false;

  GLfloat *transMat_x;
  GLfloat *transMat_y;
  GLfloat *transMat_z;
  float theta_x = 0.0f;
  float theta_y = 0.0f;
  float theta_z = 0.0f;

  GLfloat *transMat_scaling;
  float scale = 1.0f;

  GLfloat *transMat_translation;

  uint32_t last_tick = 0, current_tick, delta;

  // Camera
  Camera camera;
  Vec3 eye = {0.0f, 0.0f, 1.0f};     // cam position
  Vec3 center = {0.0f, 0.0f, -1.0f}; // position of where the cam is looking
  Vec3 up = {0.0, 1.0f, 0.0f};       // world up
  camera.position = eye;
  camera.direction = center;
  camera.up = up;
  camera.movement_speed = 0.5f;

  while (!DONE) {
    current_tick = SDL_GetTicks();
    last_tick = current_tick;
    delta = SDL_TICKS_PASSED(last_tick, current_tick);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_MOUSEMOTION:
        camera.pitch += event.motion.yrel;
        camera.yaw += event.motion.xrel;
        camera.direction = camera_direction(camera);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          camera.yaw -= camera.movement_speed * delta;
          camera.direction = camera_direction(camera);
          break;
        case SDLK_RIGHT:
          camera.yaw += camera.movement_speed * delta;
          camera.direction = camera_direction(camera);
          break;
        case SDLK_UP:
          camera.pitch += camera.movement_speed * delta;
          camera.direction = camera_direction(camera);
          break;
        case SDLK_DOWN:
          camera.pitch -= camera.movement_speed * delta;
          camera.direction = camera_direction(camera);
          break;
        case SDLK_f:
          scale -= 0.1f;
          break;
        case SDLK_g:
          scale += 0.1f;
          break;
        case SDLK_w:
          camera.position =
              vec_scalar_multiplication(camera_move_forward(camera), delta);
          break;
        case SDLK_a:
          camera.position =
              vec_scalar_multiplication(camera_move_left(camera), delta);
          break;
        case SDLK_s:
          camera.position =
              vec_scalar_multiplication(camera_move_backward(camera), delta);
          break;
        case SDLK_d:
          camera.position =
              vec_scalar_multiplication(camera_move_right(camera), delta);
          break;
        case SDLK_q:
          cubes[0].position.z -= 0.2f;
          break;
        case SDLK_e:
          cubes[0].position.z += 0.2f;
          break;
        }
        break;
      case SDL_QUIT:
        DONE = true;
        break;
      }
    }
    // Draw the object
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK);    // cull back face
    glFrontFace(GL_CCW);    // GL_CCW for counter clock-wise

    glClearColor(0.4f, 0.3f, 0.7f, 1.0f);

    // View
    GLfloat *transMat_camera_view =
        FPSViewRH(camera.position, camera.pitch, camera.yaw);
    glUniformMatrix4fv(camera_view, 1, GL_FALSE, transMat_camera_view);

    for (size_t i = 0; i < numCubes; i++) {
      Cube cube = cubes[i];
      // Model
      transMat_scaling = scaling_matrix(scale);
      glUniformMatrix4fv(transform_scaling, 1, GL_TRUE, transMat_scaling);

      transMat_translation =
          translation_matrix(cube.position.x, cube.position.y, cube.position.z);
      glUniformMatrix4fv(transform_translation, 1, GL_TRUE,
                         transMat_translation);

      transMat_x = transformation_matrix_x(theta_x);
      glUniformMatrix4fv(transform_x, 1, GL_TRUE, transMat_x);
      transMat_y = transformation_matrix_y(theta_y);
      glUniformMatrix4fv(transform_y, 1, GL_TRUE, transMat_y);
      transMat_z = transformation_matrix_z(theta_z);
      glUniformMatrix4fv(transform_z, 1, GL_TRUE, transMat_z);

      // GOTTA BIND THE VAO TO TELL OPENGL WHERE THE INDICES ARE FROM
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
    }
    SDL_GL_SwapWindow(window);
  }

  free(transMat_x);
  free(transMat_y);
  free(transMat_z);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  return 0;
}

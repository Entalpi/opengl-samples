#ifndef MAIN_H
#define MAIN_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

#include "math/vector.h"

typedef struct { GLfloat r, g, b, a; } Color4;
typedef struct {
  Point3 position;
  Color4 color;
  Point2 texCoord;
} Vertex;
typedef struct { Vertex vertices[4]; } Quad;
typedef struct {
  Quad quads[2];
  Point3 position; // Center point of the cube
} Cube;

typedef struct {
  Vec3 direction;
  Vec3 position;
  Vec3 up;
  GLfloat pitch;
  GLfloat yaw;
  float movement_speed;
} Camera;

Cube new_cube(Color4 *colors);
Vertex new_vertex(Point3 point, Color4 color, Point2 texCoord);
Quad new_quad(Point3 *points, Color4 *colors, Point2 *texCoords);
GLfloat *quad_to_floats(Quad *quad);
const GLchar *load_shader_source(char *filename);
GLfloat *transformation_matrix_x(float theta);
GLfloat *transformation_matrix_y(float theta);
GLfloat *transformation_matrix_z(float theta);
GLfloat *scaling_matrix(float scale);
GLfloat *translation_matrix(float x, float y, float z);

// Colors
Color4 new_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  Color4 color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  return color;
}
Color4 new_color_red() { return new_color(1.0f, 0.0f, 0.0f, 1.0f); }
Color4 new_color_blue() { return new_color(0.0f, 1.0f, 0.0f, 1.0f); }
Color4 new_color_green() { return new_color(0.0f, 0.0f, 1.0f, 1.0f); }
Color4 new_color_yellow() { return new_color(1.0f, 1.0f, 0.0f, 1.0f); }

#endif

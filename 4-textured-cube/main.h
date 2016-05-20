#ifndef MAIN_H
#define MAIN_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <stdlib.h> // calloc, free
#include <stdbool.h>
#include <math.h>
// #include "libs/SOIL.h"
#include <SDL2/SDL_image.h>

typedef struct { GLfloat x, y, z; } Point3;
typedef struct { GLfloat x, y; } Point2;
typedef struct { GLfloat r, g, b, a; } Color4;
typedef struct {
  Point3 position;
  Color4 color;
  // Point2 texCoord;
} Vertex;
typedef struct { Vertex vertices[6]; } Quad;
typedef struct { Quad quads[6]; } Cube;

Cube new_cube(Color4 *colors);
Vertex new_vertex(Point3 point, Color4 color);
Quad new_quad(Point3 *points, Color4 *colors);
GLfloat *quad_to_floats(Quad *quad);
Point3 new_point(GLfloat x, GLfloat y, GLfloat z);
Color4 new_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
Color4 new_color_red() { return new_color(1.0f, 0.0f, 0.0f, 1.0f); }
Color4 new_color_blue() { return new_color(0.0f, 1.0f, 0.0f, 1.0f); }
Color4 new_color_green() { return new_color(0.0f, 0.0f, 1.0f, 1.0f); }
Color4 new_color_yellow() { return new_color(1.0f, 1.0f, 0.0f, 1.0f); }
const GLchar *load_shader_source(char *filename);
GLfloat *transformation_matrix_x(float theta);
GLfloat *transformation_matrix_y(float theta);
GLfloat *transformation_matrix_z(float theta);

#endif

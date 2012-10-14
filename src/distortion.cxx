#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include <cmath>

#include "distortion.hxx"

using namespace std;

Distortion::Distortion() {}
void Distortion::xform(float*) const {}

void Distortion::v(float x, float y, float z) const {
  float f[3] = {x,y,z};
  xform(f);
  glVertex3fv(f);
}

void Distortion::t(float x, float y, float z) const {
  float v[3] = {x,y,z};
  xform(v);
  glTranslatef(v[0], v[1], v[2]);
  //We also need to rotate the object so "up" is still correct
  //Project a (0,1,z) vector to the same Z coordinate, then subtract out what
  //projecting (0,0,z) gives us. The result is the "up" vector, which must be
  //rotated wrt the expected (0,1,0).
  //The axis is the cross product between them, and the angle can be derived
  //from the dot product.
  float base[] = { 0,0,z };
  float up  [] = { 0,1,z };
  xform(base);
  xform(up);
  for (unsigned i = 0; i < 3; ++i) up[i] -= base[i];

  float cross[] = {
    1*up[2] - 0*up[1],
    0*up[2] - 0*up[0],
    0*up[1] - 1*up[0],
  };
  float dot = up[1]; //Dotted with (0,1,0)
  float angle = acos(dot /* up is a unit vector */);
  if (angle == angle) {
    //Convert angle to degrees for OpenGL
    angle *= 360.0f / 2.0f / PI;
    glRotatef(angle, cross[0], cross[1], cross[2]);
  }
}

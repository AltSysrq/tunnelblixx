#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <list>

#include "distortion.hxx"
#include "tunnel.hxx"

using namespace std;

#define SECTION_LEN (Tunnel::gsqlen*8)
#define NUM_SECTIONS (Tunnel::gridLength/8)

#define SUDDEN_RESET_PROB 0.05f

Distortion::Section Distortion::Section::mutate(float violence) {
  float mutation = min(100.0f, violence) / 300.0f;
  float maxValue = max(20.0f, 200.0f/(100.0f-min(99.0f,violence)));
  Section sec = { SECTION_LEN, 0, 0, 0 };
  if (rand() < RAND_MAX*SUDDEN_RESET_PROB)
    return sec;

  sec.roll  = roll  + mutation*(rand()/(float)RAND_MAX - 0.5f);
  sec.pitch = pitch + mutation*(rand()/(float)RAND_MAX - 0.5f);
  sec.yaw   = yaw   + mutation*(rand()/(float)RAND_MAX - 0.5f);

  if (sec.roll < -maxValue) sec.roll = -maxValue;
  if (sec.roll > maxValue) sec.roll = maxValue;
  if (sec.pitch < -maxValue) sec.pitch = -maxValue;
  if (sec.pitch > maxValue) sec.pitch = maxValue;
  if (sec.yaw < -maxValue) sec.yaw = -maxValue;
  if (sec.yaw > maxValue) sec.yaw = maxValue;
  return sec;
}

Distortion::Distortion()
: convulsionMult(1)
{
  Section zero = { SECTION_LEN, 0, 0, 0 };
  sections.push_back(zero);
  refill(2.0f);
}

void Distortion::xform(float* vec) const {
  float x = vec[0], y = vec[1], z = vec[2];
  float dist = -z;

  //Move x and y into zero-centred coordinates for transformation
  x -= 0.5f;
  y -= 0.5f;

  //We need to apply the sections in reverse order;
  //start by finding the last one to apply
  list<Section>::const_iterator it = sections.begin();
  float distLeft = dist;
  while (it != sections.end() && distLeft > 0)
    distLeft -= it++->distanceLeft;

  //Transform
  while (it != sections.begin()) {
    const Section& s(*--it);
    float ad; //applied distance

    /* If distLeft is negative, the last segment only applies to a portion of
     * the distance for that section. Otherwise, it applies to the whole
     * section.
     */
    if (distLeft <= 0) {
      ad = s.distanceLeft + distLeft;
      distLeft = 1;
    } else {
      ad = s.distanceLeft;
    }

    ad *= convulsionMult;

    x = x*cos(s.yaw*ad)*cos(s.roll*ad)
      - y*sin(s.roll*ad)
      - z*sin(s.yaw*ad);
    y = y*cos(s.roll*ad)*cos(s.pitch*ad)
      - z*sin(s.pitch*ad)
      + x*sin(s.roll*ad);
    z = z*cos(s.yaw*ad)*cos(s.pitch*ad)
      + x*sin(s.yaw*ad)
      + y*sin(s.yaw*ad);
  }

  //Done
  vec[0] = x+0.5f;
  vec[1] = y+0.5f;
  vec[2] = z;
}

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
  float dot = min(1.0f, max(-1.0f, up[1])); //Dotted with (0,1,0)
  float angle = acos(dot /* up is a unit vector */);
  if (angle == angle) {
    //Convert angle to degrees for OpenGL
    angle *= 360.0f / 2.0f / PI;
    glRotatef(angle, cross[0], cross[1], cross[2]);
  }
}

void Distortion::translateZ(float offset, float violence) {
  offset = -offset;
  while (sections.front().distanceLeft < offset) {
    offset -= sections.front().distanceLeft;
    sections.pop_front();
  }

  sections.front().distanceLeft -= offset;
  refill(violence);
}

void Distortion::refill(float violence) {
  while (sections.size() < NUM_SECTIONS)
    sections.push_back(sections.back().mutate(violence));
}

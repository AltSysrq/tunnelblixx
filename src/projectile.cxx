#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include <algorithm>
#include <cmath>

#include "game_object.hxx"
#include "distortion.hxx"
#include "projectile.hxx"

using namespace std;

#define MAX_LEN 16.0f
#define SEG_SZ 0.25f

Projectile::Projectile(GameField* field, const Distortion* dist,
                       const GameObject* parent_,
                       float x_, float y_, float z_,
                       float direction_, float speed_)
: GameObject(field, x_, y_, z_, radius, radius, radius),
  distortion(dist),
  parent(parent_),
  direction(direction_),
  alpha(1.0f),
  initz(z_),
  speed(speed_),
  scanning(true)
{
  //Scan ahead (up to MAX_LEN) for objects to collide with
  while (scanning && fabs(z-initz) < MAX_LEN)
    moveTo(x, y, z+direction*SEG_SZ, true);

  //Ensure z is the further coordinate
  if (z > initz)
    swap(z, initz);
  //Make initz relative to z
  initz -= z;

  scanning = false;
}

void Projectile::update(float et) {
  //Fade out in a quarter second
  alpha -= et*4.0f;
  alive = (alpha > 0);

  //Retract beam
  if (direction > 0) {
    //z < z+initz, increase z and decrease initz
    z += speed*et;
    initz -= speed*et;
    if (initz <= 0) alive = false;
  } else {
    //z < z+initz, decrease initz
    initz -= speed*et;
    if (initz <= 0) alive = false;
  }
}

void Projectile::draw() {
  float begin = min(z+initz, z);
  float end = max(z+initz, z);

  glColor4f(1, 1, 0, alpha);
  glBegin(GL_QUADS);
  for (float off = begin; off < end; off += SEG_SZ) {
    float len = min(end-off, SEG_SZ);
    distortion->v(x-radius, y, off);
    distortion->v(x+radius, y, off);
    distortion->v(x+radius, y, off+len);
    distortion->v(x-radius, y, off+len);

    distortion->v(x, y-radius, off);
    distortion->v(x, y+radius, off);
    distortion->v(x, y+radius, off+len);
    distortion->v(x, y-radius, off+len);

    distortion->v(x-radius, y, off+len);
    distortion->v(x, y+radius, off+len);
    distortion->v(x+radius, y, off+len);
    distortion->v(x, y-radius, off+len);
  }
  glEnd();
}

void Projectile::collideWith(GameObject* that) {
  if (that != parent && scanning) {
    scanning = false;
    that->collideWithProjectile(this);
  }
}

bool Projectile::isCollideable() const {
  return scanning;
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <cmath>

#include <SDL_opengl.h>

#include "confetti.hxx"
#include "distortion.hxx"

#define NUM_CONFETTI (sizeof(confetti)/sizeof(Confettus))
#define COLOUR_VAR 0.35f
#define GRAVITY -2.5f
#define EXPV 1.0f
#define EXPYBIAS 1.65f
#define FRICTION 0.5f
#define SZ 0.1f

using namespace std;

static inline float rnd() {
  return rand() / (float)RAND_MAX * 2.0f - 1.0f;
}

Confetti::Confetti(GameField* field,
                   float x, float y, float z,
                   float r, float g, float b,
                   Distortion* dist)
: GameObject(field, x, y, z, 0, 0, 0),
  distortion(dist)
{
  for (unsigned i = 0; i < NUM_CONFETTI; ++i) {
    Confettus& c(confetti[i]);
    c.x = c.y = c.z = 0;
    c.vx = rnd()*EXPV;
    c.vy = rnd()*EXPV + EXPYBIAS;
    c.vz = rnd()*EXPV;
    c.rot = rnd()*360;
    c.vrot = rnd()*360*20;
    c.rx = rnd();
    c.ry = rnd();
    c.rz = rnd();
    c.r = r + rnd()*COLOUR_VAR;
    c.g = g + rnd()*COLOUR_VAR;
    c.b = b + rnd()*COLOUR_VAR;
  }
}

void Confetti::update(float et) {
  bool anyAbovePlane = false;
  for (unsigned i = 0; i < NUM_CONFETTI; ++i) {
    Confettus& c(confetti[i]);
    c.x += c.vx * et;
    c.y += c.vy * et;
    c.z += c.vz * et;
    c.vy += GRAVITY*et;
    c.vx *= pow(FRICTION, et);
    c.vy *= pow(FRICTION, et);
    c.vz *= pow(FRICTION, et);
    c.rot += c.vrot*et;

    anyAbovePlane |= y + c.y > 0;
  }

  alive = anyAbovePlane;
}

void Confetti::draw() {
  glPushMatrix();
  distortion->t(x, y, z);
  for (unsigned i = 0; i < NUM_CONFETTI; ++i) {
    const Confettus& c(confetti[i]);
    glPushMatrix();
    glTranslatef(c.x, c.y, c.z);
    glRotatef(c.rot, c.rx, c.ry, c.rz);
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(+SZ/2, 0);
    glVertex2f(-SZ/2, +SZ/2);
    glVertex2f(-SZ/2, -SZ/2);
    glEnd();
    glPopMatrix();
  }
  glPopMatrix();
}

void Confetti::collideWith(GameObject*) {}
bool Confetti::isCollideable() const { return false; }

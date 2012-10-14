#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

#include "game_object.hxx"
#include "game_field.hxx"

using namespace std;

GameObject::GameObject(GameField* field_,
                       float x_, float y_, float z_,
                       float w_, float h_, float l_)
: x(x_), y(y_), z(z_), w(w_), h(h_), l(l_),
  alive(true), field(field_)
{
}

GameObject::~GameObject() {}

bool GameObject::moveTo(float x, float y, float z, bool force) {
  //Snap to boundaries
  if (x - w/2 < 0) x = w/2;
  if (x + w/2 > 1) x = 1 - w/2;
  if (y - h/2 < 0) y = h/2;

  float dx = fabs(this->x-x);
  float dy = fabs(this->y-y);
  float dz = fabs(this->z-z);
  float cx = (x+this->x)/2;
  float cy = (y+this->y)/2;
  float cz = (z+this->z)/2;

  if (this->isCollideable()) {
    for (GameField::iterator it = field->begin();
         it != field->end(); ++it) {
      GameObject* that = *it;
      if (that != this) {
        if (overlap(cx, w+dx, that->x, that->w) &&
            overlap(cy, h+dy, that->y, that->h) &&
            overlap(cz, l+dz, that->z, that->l) &&
            that->isCollideable()) {
          if (force) {
            this->collideWith(that);
            that->collideWith(this);
          } else {
            return false;
          }
        }
      }
    }
  }

  this->x = x;
  this->y = y;
  this->z = z;
  return true;
}

bool GameObject::overlap(float x0, float w0, float x1, float w1) {
  return fabs(x0-x1) < (w0+w1)/2.0f;
}

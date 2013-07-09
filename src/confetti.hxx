#ifndef CONFETTI_HXX_
#define CONFETTI_HXX_

#include "game_object.hxx"

class Distortion;

class Confetti: public GameObject {
  struct Confettus {
    float x, y, z, vx, vy, vz, rot, vrot, rx, ry, rz, r, g, b;
  } confetti[8];
  Distortion*const distortion;

public:
  Confetti(GameField* field,
           float x, float y, float z,
           float r, float g, float b,
           Distortion* dist);
  virtual void update(float);
  virtual void draw();
  virtual void collideWith(GameObject*);
};

#endif /* CONFETTI_HXX_ */

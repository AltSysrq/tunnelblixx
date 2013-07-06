#ifndef BEETLE_HXX_
#define BEETLE_HXX_

#include "enemy.hxx"


class Beetle: public Enemy {
  float vx;

public:
  Beetle(Game&);
  virtual void update(float);

  virtual const float* getColour() const;
  virtual float getPulseSpeed() const;
  virtual unsigned getAward() const;
  virtual const char* getDeathSound() const;
};

#endif /* BEETLE_HXX_ */

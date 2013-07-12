#ifndef FLEA_HXX_
#define FLEA_HXX_

#include "enemy.hxx"

class Flea: public Enemy {
  float vy, vz;
  bool blocked;
public:
  Flea(Game&);
  virtual void update(float);
  virtual const float* getColour() const;
  virtual float getPulseSpeed() const;
  virtual const char* getDeathSound() const;
  virtual unsigned getAward() const;
};

#endif /* FLEA_HXX_ */

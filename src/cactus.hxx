#ifndef CACTUS_HXX_
#define CACTUS_HXX_

#include "enemy.hxx"

class Cactus: public Enemy {
public:
  Cactus(Game&);
  virtual void update(float);

  virtual const float* getColour() const;
  virtual float getPulseSpeed() const;
  virtual unsigned getAward() const;
  virtual const char* getDeathSound() const;
};

#endif /* CACTUS_HXX_ */

#ifndef FIRE_FERN_HXX_
#define FIRE_FERN_HXX_

#include "enemy.hxx"

class FireFern: public Enemy {
public:
  FireFern(Game&);
  virtual void update(float);

  virtual const float* getColour() const;
  virtual float getPulseSpeed() const;
  virtual unsigned getAward() const;
  virtual const char* getDeathSound() const;
};

#endif /* FIRE_FERN_HXX_ */

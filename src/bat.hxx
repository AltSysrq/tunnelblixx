#ifndef BAT_HXX_
#define BAT_HXX_

#include "enemy.hxx"

class Bat: public Enemy {
  bool isFlying;
  const Game& game;
  float vy, vz;

public:
  Bat(Game&);
  virtual void update(float et);
  virtual void collideWithProjectile(Projectile*);

  virtual const float* getColour() const;
  virtual float getPulseSpeed() const;
  virtual const char* getDeathSound() const;
  virtual unsigned getAward() const;
};

#endif /* BAT_HXX_ */

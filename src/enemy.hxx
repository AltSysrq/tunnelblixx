#ifndef ENEMY_HXX_
#define ENEMY_HXX_

#include "modelled_object.hxx"

class Game;

/**
 * Common base class for enemy objects.
 *
 * Enemies lose one hit point every time they are hit by a projectile, and die
 * when their hit points reach zero. At this point, a pair of pulses is added
 * to the tunnel, and the player is awarded points. They kill the player on
 * contact.
 */
class Enemy: public ModelledObject {
  Game& game;
  unsigned hitPoints;

protected:
  Enemy(Game&, const Model&, unsigned hp);

public:
  virtual void collideWith(GameObject*);

protected:
  virtual const float* getColour() const = 0;
  virtual float getPulseSpeed() const = 0;
  virtual unsigned getAward() const = 0;
  virtual const char* getDeathSound() const = 0;
};

#endif /* ENEMY_HXX_ */

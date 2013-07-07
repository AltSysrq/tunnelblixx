#ifndef PILLAR_HXX_
#define PILLAR_HXX_

#include "modelled_object.hxx"

class Game;

/**
 * The Pillar is an obstacle which kills the player on contact and is
 * indestructable. It is occasionally spawned in the player's path to force the
 * player to move around.
 */
class Pillar: public ModelledObject {
public:
  Pillar(Game&);
  virtual void update(float);
  virtual void collideWith(GameObject*);
  virtual void collideWithPlayer(Player*);
};

#endif /* PILLAR_HXX_ */

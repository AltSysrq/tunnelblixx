#ifndef HURTLE_HXX_
#define HURTLE_HXX_

#include "modelled_object.hxx"

class Game;

class Hurtle: public ModelledObject {
public:
  Hurtle(Game&);
  virtual void update(float);
  virtual void collideWith(GameObject*);
  virtual void collideWithPlayer(Player*);
};

#endif /* HURTLE_HXX_ */

#ifndef PLAYER_HXX_
#define PLAYER_HXX_

#include "modelled_object.hxx"

class Player: public ModelledObject {
  float vy;
  void (*notifyOnDeath)(void*);
  void* notifyObject;

public:
  Player(GameField* field, const Distortion*,
         void (*)(void*), void*);
  void kill();

  virtual void update(float et);
  virtual void collideWith(GameObject*);

  void move(float x);
  void advance(float z, float offset);
  void jump();
};

#endif /* PLAYER_HXX_ */

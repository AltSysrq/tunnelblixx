#ifndef PLAYER_HXX_
#define PLAYER_HXX_

#include "modelled_object.hxx"

class Player: public ModelledObject {
  float vy;
  void (*notifyOnDeath)(void*);
  void* notifyObject;

  //Delete later when music-based shot timing is used
  float timeUntilShot;

public:
  Player(GameField* field, const Distortion*, float offset,
         void (*)(void*), void*);
  void kill();

  virtual void update(float et);
  virtual void collideWith(GameObject*);

  void move(float x);
  void advance(float z, float offset);
  void jump();
};

#endif /* PLAYER_HXX_ */

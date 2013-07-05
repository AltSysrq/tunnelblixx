#ifndef PLAYER_HXX_
#define PLAYER_HXX_

#include "modelled_object.hxx"

class Player: public ModelledObject {
  friend class Game;

  float vy;
  void (*notifyOnDeath)(void*);
  void* notifyObject;
  float shotSpeed;

  float prevAmplitude, newAmplitude;
  float shotThreshhold;

public:
  Player(GameField* field, const Distortion*, float offset,
         void (*)(void*), void*);
  void kill();

  virtual void update(float et);
  virtual void collideWith(GameObject*);

  void move(float x);
  void advance(float z, float offset);
  void jump();
  void setAmplitude(float);
};

#endif /* PLAYER_HXX_ */

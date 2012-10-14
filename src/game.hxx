#ifndef GAME_HXX_
#define GAME_HXX_

#include "tunnel.hxx"
#include "distortion.hxx"
#include "game_field.hxx"

class Player;

class Game {
  Tunnel tunnel;
  Distortion distortion;
  GameField field;
  Player* player;

  float speed;

public:
  Game();
  ~Game();

  void update(float);
  void configureGL();
  void draw();
  void motion(float,float);
  void button(unsigned);
  bool running() const;
  float getSpawnDistance() const;

private:
  float getNearClippingPlane() const;
  static void playerDeath(void*);
};

#endif /* GAME_HXX_ */

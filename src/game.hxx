#ifndef GAME_HXX_
#define GAME_HXX_

#include "tunnel.hxx"
#include "distortion.hxx"
#include "game_field.hxx"
#include "enemy_factory.hxx"
#include "mixer.hxx"

class Player;

class Game {
public:
  Tunnel tunnel;
  Distortion distortion;
  GameField field;
  Mixer mixer;
  float currentConvulsion, targetConvulsion;
  float playerAmplitude;

private:
  EnemyFactory enemyFactory;
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
  float getPlayerX() const;

  void startMusic(const char*const*, unsigned);

  void amplitude(float);

private:
  float getNearClippingPlane() const;
  static void playerDeath(void*);
};

#endif /* GAME_HXX_ */

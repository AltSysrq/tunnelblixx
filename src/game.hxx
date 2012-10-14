#ifndef GAME_HXX_
#define GAME_HXX_

#include "tunnel.hxx"
#include "distortion.hxx"

class Game {
  Tunnel tunnel;
  Distortion distortion;

public:
  Game();
  ~Game();

  void update(float);
  void configureGL();
  void draw();
  void motion(float,float);
  void button(unsigned);
  bool running() const;
};

#endif /* GAME_HXX_ */

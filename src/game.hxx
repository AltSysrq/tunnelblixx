#ifndef GAME_HXX_
#define GAME_HXX_

#include "tunnel.hxx"

class Game {
  Tunnel tunnel;

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

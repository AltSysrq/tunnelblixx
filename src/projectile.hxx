#ifndef PROJECTILE_HXX_
#define PROJECTILE_HXX_

#include "game_object.hxx"

class Distortion;

class Projectile: public GameObject {
  const Distortion*const distortion;
  const GameObject*const parent;
  float direction;
  float alpha;
  float initz;
  bool scanning;

public:
  static const float radius = 0.015f;

  Projectile(GameField*, const Distortion*, const GameObject* parent,
             float x, float y, float z,
             float direction);

  virtual void update(float);
  virtual void draw();
  virtual void collideWith(GameObject*);
};

#endif /* PROJECTILE_HXX_ */

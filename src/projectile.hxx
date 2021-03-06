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
  float speed;
  bool scanning;

public:
  static const float radius = 0.015f;

  Projectile(GameField*, const Distortion*, const GameObject* parent,
             float x, float y, float z,
             float direction, float speed);

  virtual void update(float);
  virtual void draw();
  virtual void collideWith(GameObject*);
  virtual bool isCollideable() const;
  virtual bool isOpaque() const { return true; }
};

#endif /* PROJECTILE_HXX_ */

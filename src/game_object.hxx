#ifndef GAME_OBJECT_HXX_
#define GAME_OBJECT_HXX_

class GameField;
class Player;
class Projectile;

/**
 * Encapuslates the basic data and operations for arbitrary objects within the
 * game.
 */
class GameObject {
  friend class GameField;
protected:
  float x, y, z, w, h, l;
  /**
   * Whether the object is currently "alive".
   * When set to false, the object will be removed from the field after the
   * next update.
   */
  bool alive;
  /**
   * The GameField in which this object lives.
   */
  GameField*const field;

  GameObject(GameField*,
             float x, float y, float z,
             float w, float h, float l);
public:
  virtual ~GameObject();

  /**
   * Performs any updating necessary for this object.
   *
   * @param elapsedTime The amount of time, in seconds, since the last call to
   * update().
   */
  virtual void update(float) = 0;
  /**
   * Draws the object.
   */
  virtual void draw() = 0;
  /**
   * Notifies the object that it has collided with the given other GameObject.
   */
  virtual void collideWith(GameObject*) = 0;
  /**
   * Notifies the object that it has collided with the player.
   *
   * Default does nothing.
   */
  virtual void collideWithPlayer(Player*) {}
  /**
   * Notifies the object that it has collided with a projectile.
   *
   * Default does nothing.
   */
  virtual void collideWithProjectile(Projectile*) {}

  /**
   * Returns whether this object can collide with others.
   * Default returns true.
   */
  virtual bool isCollideable() const { return true; }

  /**
   * Returns whether this object is opaque.
   * Semitransparent objects are drawn in a separate pass, with the
   * depth-buffer read-only.
   *
   * Default returns true.
   */
  virtual bool isOpaque() const { return true; }

  bool isAlive() const { return alive; }
  float getX() const { return x; }
  float getY() const { return y; }
  float getZ() const { return z; }
  float getW() const { return w; }
  float getH() const { return h; }
  float getL() const { return l; }

protected:
  /**
   * Attempts to move to the given new coordinates. If force is false, the
   * movement only takes place if the new location is unoccupied. If force is
   * true, the movement will alway occur, triggering a collision with anything
   * that is already there.
   *
   * The distance moved is also considered for collision detection.
   *
   * @param x The new X coordinate to which to try to move
   * @param y The new Y coordinate to which to try to move
   * @param z The new Z coordinate to which to try to move
   * @param force Whether to move even if a collision occurs
   * @return Whether any movement actually occurred.
   */
  bool moveTo(float x, float y, float z, bool force);

private:
  static bool overlap(float x0, float w0, float x1, float w1);
};

#endif /* GAME_OBJECT_HXX_ */

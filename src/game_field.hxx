#ifndef GAME_FIELD_HXX_
#define GAME_FIELD_HXX_

#include <list>

class GameObject;

/**
 * Maintains a list of GameObjects in existence, and manages forwarding calls
 * to them, etc.
 */
class GameField {
  std::list<GameObject*> objects, toSpawn;

public:
  ~GameField();

  /**
   * Updates all objects, then adds newly-spawned objects to the list.
   *
   * @param elapsedTime The time, in seconds, since the previous call to
   * update().
   */
  void update(float);

  /** Draws all objects in the field. */
  void draw() const;

  /** Enqueues the given object to be spawned after the next update. */
  void add(GameObject*);

  /** Translates all objects by the given Z amount, to keep up with the
   * player.
   *
   * Objects whose Z becomes positive are deleted.
   */
  void translateZ(float);

  typedef std::list<GameObject*>::const_iterator iterator;
  iterator begin() const;
  iterator end() const;
};

#endif /* GAME_FIELD_HXX_ */

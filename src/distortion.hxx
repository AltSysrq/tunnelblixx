#ifndef DISTORTION_HXX_
#define DISTORTION_HXX_

#include <list>

/**
 * Transforms the world according to the logical z coordinate, to make the
 * tunnel more interesting.
 */
class Distortion {
  struct Section {
    float distanceLeft, roll, pitch, yaw;
    Section mutate(float);
  };

  std::list<Section> sections;
  float convulsionMult;

public:
  Distortion();

  /**
   * Transforms the given vertex, then draws it using glVertex3fv.
   */
  void v(float x, float y, float z) const;

  /**
   * Adds an appropriate translation transform to the current GL matrix, which
   * will have an effect similar to (but much faster than) drawing each vertex
   * with v(float,float,float).
   */
  void t(float x, float y, float z) const;

  /**
   * Translates the Z axis by the given amount, to keep up with the player.
   */
  void translateZ(float off, float violence);

private:
  void xform(float*) const;
  void refill(float);
};

#endif /* DISTORTION_HXX_ */

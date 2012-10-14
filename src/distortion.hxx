#ifndef DISTORTION_HXX_
#define DISTORTION_HXX_

/**
 * Transforms the world according to the logical z coordinate, to make the
 * tunnel more interesting.
 */
class Distortion {
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

private:
  void xform(float*) const;
};

#endif /* DISTORTION_HXX_ */

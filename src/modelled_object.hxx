#ifndef MODELLED_OBJECT_HXX_
#define MODELLED_OBJECT_HXX_

#include "game_object.hxx"

class Model;
class Distortion;

/**
 * Encapsulates a GameObject which is drawn according to a non-changing
 * model. Collision boundaries are automatically calculated, and this class
 * handles drawing.
 *
 * A model is simply an array of floats whose length is a multiple of four. The
 * first float in each quadruple is zero if that quadruple is a vertex, or
 * a colour otherwise. The other three are (x,y,z) coordinates or an (r,g,b)
 * colour, as determined by the first.
 */
class ModelledObject: public GameObject {
  const Model* model;

protected:
  const Distortion*const distortion;
  float rotation;

  ModelledObject(GameField*, float, float, float,
                 const Model&,
                 const Distortion*);
  /**
   * Changes the model being used.
   *
   * This includes recalculating collision boundaries.
   */
  void setModel(const Model&);

public:
  virtual void draw();
};

#endif /* MODELLED_OBJECT_HXX_ */

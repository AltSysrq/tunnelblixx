#ifndef MODEL_HXX_
#define MODEL_HXX_

#include <SDL_opengl.h>

#define M_END 0.0f
#define M_VER 1.0f
#define M_COL 2.0f

/**
 * Encapsulates a model as an OpenGL display list.
 */
class Model {
  GLuint displayList;
  float w, h, l;

public:
  /**
   * Constructs a model with the given data. For each datum, a float is read
   * in; if it is M_END, reading stops; if it is M_VER, glVertex3f is called
   * with the next three floats; otherwise, glColor3f is called with the next
   * three floats.
   *
   * The model is normalised so that it is centred on its width/height/length,
   * if normalise is true.
   */
  Model(bool normalise, ...);

  ~Model();

  /** Draws the model. */
  void draw() const;

  /** Returns the width of the model. */
  float getW() const { return w; }
  /** Returns the height of the model. */
  float getH() const { return h; }
  /** Returns the length of the model. */
  float getL() const { return l; }
};

#endif /* MODEL_HXX_ */

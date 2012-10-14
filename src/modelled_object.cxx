#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <SDL_opengl.h>

#include "modelled_object.hxx"
#include "model.hxx"
#include "distortion.hxx"

ModelledObject::ModelledObject(GameField* field, float x, float y, float z,
                               const Model& model_, const Distortion* dist)
: GameObject(field, x, y, z, model_.getW(), model_.getH(), model_.getL()),
  model(&model_), distortion(dist), rotation(0)
{
}

void ModelledObject::setModel(const Model& model) {
  this->model = &model;
  w = model.getW();
  h = model.getH();
  l = model.getL();
}

void ModelledObject::draw() {
  glPushMatrix();
  distortion->t(x, y, z);
  glRotatef(rotation, 0, 1, 0);
  model->draw();
  glPopMatrix();
}

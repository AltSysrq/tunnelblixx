#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include <iostream>
#include <vector>
#include <cstdarg>
#include <cfloat>
#include <algorithm>

#include "model.hxx"

using namespace std;

Model::Model(bool normalise, ...)
: displayList(0)
{
  float minx, maxx, miny, maxy, minz, maxz;
  bool hasExtrema = false;
  va_list val;

  //Read in the data
  va_start(val, normalise);
  float f;
  do {
    // Why the heck is float promoted to double in varargs?
    f = va_arg(val, double);
    if (f != M_END) {
      data.push_back(f);
      for (int i = 0; i < 3; ++i)
        data.push_back(va_arg(val, double));
    }
  } while (f != M_END);
  va_end(val);

  float* begin = &data[0], * end = begin+data.size();
  //Get the dimensions
  for (float* i = begin; i != end; i += 4) {
    if (*i == M_VER) {
      if (!hasExtrema) {
        minx = maxx = i[1];
        miny = maxy = i[2];
        minz = maxz = i[3];
        hasExtrema = true;
      } else {
        minx = min(minx, i[1]);
        maxx = max(maxx, i[1]);
        miny = min(miny, i[2]);
        maxy = max(maxy, i[2]);
        minz = min(minz, i[3]);
        maxz = max(maxz, i[3]);
      }
    }
  }

  w = maxx-minx;
  h = maxy-miny;
  l = maxz-minz;

  //Normalise if requested
  if (normalise) {
    float xo = (maxx + minx) / 2;
    float yo = (maxy + miny) / 2;
    float zo = (maxz + minz) / 2;
    for (float* i = begin; i != end; i += 4) {
      if (*i == M_VER) {
        i[1] -= xo;
        i[2] -= yo;
        i[3] -= zo;
      }
    }
  }
}

Model::~Model() {
  glDeleteLists(displayList, 1);
}

void Model::draw() const {
  if (!displayList) {
    //Compile to display list
    displayList = glGenLists(1);
    cout << displayList << endl;
    glNewList(displayList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    const float* begin = &data[0], * end = begin+data.size();
    for (const float* i = begin; i != end; i += 4)
      if (*i == M_VER)
        glVertex3fv(i+1);
      else
        glColor3fv(i+1);
    glEnd();
    glEndList();
  }

  glCallList(displayList);
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <vector>

#include "fire_fern.hxx"
#include "model.hxx"

using namespace std;

#define ROT_RATE 90
#define HEIGHT 0.35f
#define IHEIGHT (HEIGHT/3.0f)
#define RADIUS 0.15f
#define IRADIUS (RADIUS/6.0f)
#define DIVS 5

static void generateModel(vector<float>& dst) {
#define C(r,g,b) do {                                           \
    dst.push_back(M_COL);                                       \
    dst.push_back(r); dst.push_back(g); dst.push_back(b);       \
  } while (0)
#define V(x,y,z) do {                                           \
    dst.push_back(M_VER);                                       \
    dst.push_back(x); dst.push_back(y); dst.push_back(z);       \
  } while (0)

  for (unsigned i = 0; i < DIVS; ++i) {
    float th0 = (float)(i*2*PI/DIVS), th1 = (float)((i+1)*2*PI/DIVS);
    float thm = (th1+th0)/2;
    C(0.8f,0,0);
    V(cos(th0)*IRADIUS, 0, sin(th0)*IRADIUS);
    V(0, IHEIGHT, 0);
    C(1,0.5f,0);
    V(cos(thm)*RADIUS, HEIGHT, sin(thm)*RADIUS);

    C(0.8f,0,0);
    V(cos(th1)*IRADIUS, 0, sin(th1)*IRADIUS);
    V(0, IHEIGHT, 0);
    C(1,0.5f,0);
    V(cos(thm)*RADIUS, HEIGHT, sin(thm)*RADIUS);

    C(0.8f,0,0);
    V(cos(th1)*IRADIUS, 0, sin(th1)*IRADIUS);
    V(cos(th0)*IRADIUS, 0, sin(th0)*IRADIUS);
    C(1,0.5f,0);
    V(cos(thm)*RADIUS, HEIGHT, sin(thm)*RADIUS);
  }
}

static Model fireFernModel(generateModel, true);

FireFern::FireFern(Game& game)
: Enemy(game, fireFernModel, 2)
{ }

void FireFern::update(float et) {
  rotation = fmod(rotation + ROT_RATE*et, 360.0f);
}

const float* FireFern::getColour() const {
  static const float colour[3] = { 0.8f, 0.1f, -0.5f };
  return colour;
}

float FireFern::getPulseSpeed() const {
  return 12;
}

unsigned FireFern::getAward() const {
  return 250;
}

const char* FireFern::getDeathSound() const {
  return "sound/firefern_death.pcm";
}

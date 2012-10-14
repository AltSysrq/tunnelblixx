#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <vector>

#include "cactus.hxx"
#include "model.hxx"

using namespace std;

#define DIVS 6
#define RADIUS 0.08f
#define BASE_RADIUS (0.85f*RADIUS)
#define HEIGHT 0.3f

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
    float th0 = i*2*PI/DIVS, th1 = (i+1)%DIVS*2*PI/DIVS;

    //Generate the base (lower 1/3)
    C(0, 0.5f, 0);
    V(BASE_RADIUS*cos(th0), 0, BASE_RADIUS*sin(th0));
    V(BASE_RADIUS*cos(th1), 0, BASE_RADIUS*sin(th1));
    C(0, 0.8f, 0);
    V(RADIUS*cos(th0), HEIGHT/3, RADIUS*sin(th0));

    V(RADIUS*cos(th0), HEIGHT/3, RADIUS*sin(th0));
    V(RADIUS*cos(th1), HEIGHT/3, RADIUS*sin(th1));
    C(0, 0.5f, 0);
    V(BASE_RADIUS*cos(th1), 0, BASE_RADIUS*sin(th1));

    //Generate the middle
    C(0, 0.8f, 0);
    V(RADIUS*cos(th0), 1*HEIGHT/3, RADIUS*sin(th0));
    V(RADIUS*cos(th1), 1*HEIGHT/3, RADIUS*sin(th1));
    V(RADIUS*cos(th0), 2*HEIGHT/3, RADIUS*sin(th0));

    V(RADIUS*cos(th0), 2*HEIGHT/3, RADIUS*sin(th0));
    V(RADIUS*cos(th1), 2*HEIGHT/3, RADIUS*sin(th1));
    V(RADIUS*cos(th1), 1*HEIGHT/3, RADIUS*sin(th1));

    //Generate the top cone
    V(RADIUS*cos(th0), 2*HEIGHT/3, RADIUS*sin(th0));
    V(RADIUS*cos(th1), 2*HEIGHT/3, RADIUS*sin(th1));
    C(0.5f, 0.95f, 0.2f);
    V(0, HEIGHT, 0);
  }

#undef V
#undef C
}

static Model cactusModel(generateModel, true);

Cactus::Cactus(Game& game)
: Enemy(game, cactusModel, 1)
{ }

void Cactus::update(float) {}

const float* Cactus::getColour() const {
  static const float colour[3] = { 0, 0.8f, 0 };
  return colour;
}

float Cactus::getPulseSpeed() const {
  return 15.0f;
}

unsigned Cactus::getAward() const {
  return 100;
}

const char* Cactus::getDeathSound() const {
  return "sound/cactus_death.pcm";
}

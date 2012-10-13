#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <SDL.h>
#include <SDL_opengl.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>

#ifdef HAVE_FENV_H
#include <fenv.h>
#endif

#include "globals.hxx"

using namespace std;

static SDL_Surface* screen;

static void init(const char*const* argv, unsigned argc);
static void run();
static void shutdown();

int main(int argc, const char*const* argv) {
  init(argv, argc);
  run();
  shutdown();
  return 0;
}

static void init(const char*const* argv, unsigned argc) {
  //Seed the PRNG
  srand(time(NULL));

  //Init SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO)) {
    printf("Unable to initialise SDL: %s\n", SDL_GetError());
    exit(1);
  }

  //Init window
  if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    printf("Unable to init SDL Video: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_WM_SetCaption("Tunnelblick", "Tunnelblick");
  const SDL_VideoInfo* vi = SDL_GetVideoInfo();
  vheight = vi->current_h / (float)vi->current_w;
  screen = SDL_SetVideoMode(vi->current_w, vi->current_h, 0,
                            SDL_OPENGL /* | SDL_FULLSCREEN */);
  if (!screen) {
    printf("Unable to create window: %s\n", SDL_GetError());
    exit(1);
  }

  //Configure OpenGL
  glViewport(0, 0, vi->current_w, vi->current_h);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  //Enable floating-point exceptions if possible, unless debugging is disabled.
  #ifdef HAVE_FEENABLEEXCEPT
  assert(-1 != feenableexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_INVALID));
  #endif
}

void run() {
  SDL_Delay(4096);
}

void shutdown() {
  SDL_Quit();
}

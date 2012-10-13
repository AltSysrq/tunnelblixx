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

#include "game.hxx"
#include "globals.hxx"

using namespace std;

static SDL_Surface* screen;
static unsigned screenw, screenh;

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
  screenw = vi->current_w;
  screenh = vi->current_h;
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
  Game game;
  bool paused = false;
  Uint32 lastUpdate = SDL_GetTicks();
  while (game.running()) {
    //Get the current time and determine how much time has elapsed since the
    //last update, then update lastUpdate
    Uint32 now = SDL_GetTicks();
    float elapsed = (now-lastUpdate) / 1000.0f;
    lastUpdate = now;
    //If not paused, update the game
    if (!paused) game.update(elapsed);

    //Draw the game
    game.configureGL();
    game.draw();
    glFinish();
    SDL_GL_SwapBuffers();

    //Pump events
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      switch (evt.type) {
      case SDL_QUIT: return;
      case SDL_KEYDOWN:
        //Space toggles pause, esc exits
        if (evt.key.keysym.sym == SDLK_SPACE)
          paused = !paused;
        else if (evt.key.keysym.sym == SDLK_ESCAPE)
          return;

      case SDL_MOUSEMOTION:
        game.motion(evt.motion.x / (float)screenw,
                    vheight - (evt.motion.y / (float)screenh)*vheight);
        break;

      case SDL_MOUSEBUTTONDOWN:
        game.button(evt.button.button);
        break;
      }
    }
  }
}

void shutdown() {
  SDL_Quit();
}

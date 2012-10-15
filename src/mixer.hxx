#ifndef MIXER_HXX_
#define MIXER_HXX_

#include <SDL.h>

#include <list>

class AudioSource;

/**
 * Performs management and mixing of audio sources, into the SDL sound system.
 */
class Mixer {
  struct Source {
    AudioSource* source;
    Sint16 volume;
  };

  std::list<Source> sources;
  bool enabled; ///< Whether sound is actually working

public:
  Mixer();
  ~Mixer();

  /**
   * Sets the paused state of the mixer.
   */
  void pause(bool paused);
  /**
   * Plays the given source at the given volume (0 is silent, 0x7FFF is
   * maximum). The AudioSource becomes owned by this Mixer after this call, and
   * will be deleted when it completes or the Mixer is destroyed.
   */
  void play(AudioSource*, Sint16 volume);

private:
  void callback_impl(Sint16*, unsigned);
  static void callback(void*, Uint8*, int);
  static SDL_AudioSpec audioSpec;
};

#endif /* MIXER_HXX_ */

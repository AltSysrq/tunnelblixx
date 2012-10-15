#ifndef SOUND_EFFECT_HXX_
#define SOUND_EFFECT_HXX_

#include <SDL.h>

#include <vector>

class Mixer;

/**
 * Encapsulates the data for a sound effect; ie, a short piece of audio which
 * is played multiple times.
 *
 * A soundeffect is loaded from a file containing raw (headerless) uncompressed
 * signed 16-bit little-endian 44100 Hz mono LPCM data. It is loaded only once
 * (at which point it is upsampled to stereo, and byte-swapped if the machine
 * byte order is big-endian).
 */
class SoundEffect {
  friend class SoundEffectPlayer;

  std::vector<Sint16> data;

  SoundEffect(const char*);

public:
  /**
   * Playes the sound effect contained in the given file at the given
   * volume. If the file has not yet been loaded, it is loaded now. If the file
   * cannot be read, a warning is printed to stderr and empty sound data is
   * used.
   */
  static void play(Mixer&, const char* filename, Sint16 volume);
};

#endif /* SOUND_EFFECT_HXX_ */

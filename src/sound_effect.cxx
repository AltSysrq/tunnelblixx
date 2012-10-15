#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL.h>

#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <algorithm>

#include "sound_effect.hxx"
#include "audio_source.hxx"
#include "mixer.hxx"

using namespace std;

//Map of filenames to SoundEffect objects, which deletes its contents on
//destruction.
class SoundEffectMap: public map<string, SoundEffect*> {
public:
  ~SoundEffectMap() {
    for (const_iterator it = begin(); it != end(); ++it)
      delete it->second;
  }
} static soundEffectMap;

class SoundEffectPlayer: public AudioSource {
  const SoundEffect*const effect;
  unsigned ix;

public:
  SoundEffectPlayer(const SoundEffect* se)
  : effect(se), ix(0)
  { }

  virtual signed getAudio(Sint16* dst, unsigned len) {
    if (ix >= effect->data.size())
      return -1;

    if (len > effect->data.size() - ix)
      len = effect->data.size() - ix;

    memcpy(dst, (&effect->data[0])+ix, len*2);
    ix += len;
    return len;
  }
};

SoundEffect::SoundEffect(const char* filename) {
  FILE* in = fopen(filename, "rb");
  if (!in) {
    fprintf(stderr, "WARN: Could not read sound effect %s: %s\n",
            filename, strerror(errno));
    return;
  }

  Sint16 buffer[4096];
  unsigned read;
  while (!feof(in) && !ferror(in)) {
    //Read the raw data
    read = fread(buffer, 2, sizeof(buffer)/2, in);

    //If this system is big-endian, byteswap everything
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    for (unsigned i = 0; i < read; ++i) {
      unsigned char* raw = (unsigned char*)(buffer+i);
      swap(raw[0], raw[1]);
    }
#endif

    //Upsample to stereo and add to data
    data.reserve(data.size() + read*2);
    for (unsigned i = 0; i < read; ++i) {
      data.push_back(buffer[i]);
      data.push_back(buffer[i]);
    }
  }

  if (ferror(in))
    fprintf(stderr, "WARN: Read error for sound effect %s: %s\n",
            filename, strerror(errno));

  fclose(in);
}

void SoundEffect::play(Mixer& mixer, const char* filename, Sint16 volume) {
  string strname(filename);
  if (!soundEffectMap.count(strname))
    soundEffectMap[strname] = new SoundEffect(filename);

  mixer.play(new SoundEffectPlayer(soundEffectMap[strname]), volume);
}

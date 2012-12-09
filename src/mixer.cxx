#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL.h>

#include <list>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "mixer.hxx"
#include "audio_source.hxx"

using namespace std;

#define BUFFER_SIZE 4096
SDL_AudioSpec Mixer::audioSpec = {
  AUDIO_SAMPLE_RATE, AUDIO_S16SYS, 2, 0, BUFFER_SIZE, 0 /* padding */,
  2*BUFFER_SIZE,
  &callback, NULL /* populate in constructor */
};

Mixer::Mixer() {
  //Set the userdata for the SDL Audio callback
  audioSpec.userdata = this;
  if (-1 == SDL_OpenAudio(&audioSpec, NULL)) {
    enabled = false;
    cerr << "WARN: Could not open audio device: "
         << SDL_GetError() << endl;
  } else {
    enabled = true;
    pause(false);
  }
}

Mixer::~Mixer() {
  if (enabled)
    SDL_CloseAudio();

  for (list<Source>::const_iterator it = sources.begin();
       it != sources.end(); ++it)
    delete it->source;
}

void Mixer::pause(bool paused) {
  if (enabled)
    SDL_PauseAudio(paused);
}

void Mixer::play(AudioSource* source, Sint16 volume) {
  if (enabled) {
    Source s = { source, volume };
    SDL_LockAudio();
    sources.push_back(s);
    SDL_UnlockAudio();
  } else {
    //No sound available, so just destroy the source immediately
    delete source;
  }
}

void Mixer::callback(void* mixer, Uint8* stream, int len) {
  //We know that the audio data consists of 16-bit samples in machine byte
  //order, so we can safely cast the stream to the appropriate data size and
  //halve the length
  ((Mixer*)mixer)->callback_impl((Sint16*)stream, len/2);
}

static vector<Sint16> tmpbuf(BUFFER_SIZE*2);
void Mixer::callback_impl(Sint16* dst, unsigned len) {
  //Ensure the temporary buffer is big enough
  if (tmpbuf.size() < len)
    tmpbuf.resize(len);

  //Set destination to all zeroes for the base case
  memset(dst, 0, len*2);

  //Run through each AudioSource
  for (list<Source>::iterator it = sources.begin();
       it != sources.end(); ++it) {
    //Retrieve the source's data
    signed returned = it->source->getAudio(&tmpbuf[0], len);
    if (returned == -1) {
      //This source is going away
      delete it->source;
      it = sources.erase(it);
    }

    //Adjust volume and mix in
    for (signed i = 0; i < returned; ++i) {
      signed curr = dst[i];
      signed neu = tmpbuf[i];
      neu *= it->volume;
      neu >>= 15;
      curr += neu;
      //Clamp to valid Sint16 range
      if (curr < -32768)
        curr = -32768;
      if (curr >  32767)
        curr = 32767;
      //Write to destination
      dst[i] = (Sint16)curr;
    }
  }
}

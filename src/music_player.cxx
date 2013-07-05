#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL.h>

#include <iostream>
#include <vector>
#include <cstring>

#include "music_player.hxx"
#include "audio_source.hxx"
#include "vorbis_decoder.hxx"

using namespace std;

unsigned MusicDecoder::select(Sint16* dst, unsigned lc, unsigned rc, unsigned n,
                              unsigned len) const {
  unsigned total = 0;
  //If there are any partial samples at the end, just drop them
  //(Hence i+n <= len)
  for (unsigned i = 0; i+n <= len; i += n) {
    *dst++ = tempBuff[i+lc];
    *dst++ = tempBuff[i+rc];
    ++total;
  }

  return total * 2;
}

unsigned MusicDecoder::resample(Sint16* buff, unsigned len, unsigned maxlen,
                                unsigned rate) const {
  if (rate == AUDIO_SAMPLE_RATE)
    return len;

  float ratio = AUDIO_SAMPLE_RATE / (float)rate;
  float iratio = 1.0f / ratio;
  unsigned outlen = (unsigned)(len*ratio);

  //Ensure outlen is under the maximum and ends on a sample boundary.
  if (outlen > maxlen)
    outlen = maxlen;
  else if (outlen & 1)
    ++outlen;

  if (rate < AUDIO_SAMPLE_RATE) {
    //We'll be expanding the stream, so work right to left
    for (unsigned i = outlen-2; i < outlen; i -= 2) {
      unsigned src = i * iratio;
      if (src & 1) ++src;
      if (src != i)
        memcpy(buff+src, buff+i, 2 * sizeof(*buff));
    }
  } else {
    //Contracting stream, work from left to right
    for (unsigned i = 0; i < outlen; i += 2) {
      unsigned src = i * iratio;
      if (src & 1) --src;
      if (src != i)
        memcpy(buff+i, buff+src, 2 * sizeof(*buff));
    }
  }

  return outlen;
}

unsigned MusicDecoder::ensureCapacity(unsigned len, unsigned channels,
                                      unsigned rate) {
  unsigned samples = len / 2;
  unsigned samplePoints = channels * samples;

  float ratio = rate / (float)AUDIO_SAMPLE_RATE;
  unsigned outlen = (unsigned)(samplePoints * ratio);
  //Ensure we request a whole number of samples
  outlen = (outlen*channels + channels - 1) / channels;

  if (tempBuff.size() < outlen)
    tempBuff.resize(outlen);

  return outlen;
}

MusicPlayer::MusicPlayer(const char*const* files_,
                         unsigned numFiles_,
                         void (*cb)(void*, float),
                         void* ud)
: decoder(NULL),
  amplitudeCallback(cb),
  callbackUserdata(ud),
  files(files_),
  numFiles(numFiles_),
  fileIx(0)
{ }

MusicPlayer::~MusicPlayer() {
  if (decoder)
    delete decoder;
}

struct {
  const char* extension;
  MusicDecoder* (*open)(const char*);
} static decoders[] = {
  { "ogg", openVorbisDecoder },
  { NULL, NULL }
};

signed MusicPlayer::getAudio(Sint16* dst, unsigned len) {
  //Die immediately if the file list is empty
  if (0 == numFiles)
    return -1;

  //If there is currently no decoder, start decoding the next file
  if (!decoder) {
    const char* path = files[fileIx++];
    fileIx %= numFiles;

    //Find the extension
    const char* ext = path;
    for (const char* i = path; *i; ++i)
      if (*i == '.')
        ext = i+1;

    //Find a decoder for the file
    //Note: This won't handle non-lowercase extensions, but few people use those
    //for oggs and mp3s anyway
    for (unsigned i = 0; decoders[i].extension; ++i) {
      if (0 == strcmp(ext, decoders[i].extension)) {
        decoder = decoders[i].open(path);
        goto have_decoder;
      }
    }

    //No decoder for this extension
    cerr << "Note: File type " << ext << " unknown" << endl;
  }

  have_decoder:

  //If opening that was unsuccessful, return no audio this frame and try again
  //next frame
  if (!decoder) return 0;

  //Get data from the decoder
  signed ret = decoder->decode(dst, len);
  if (ret == -1) {
    //That decoder's done
    delete decoder;
    decoder = NULL;
    ret = 0;
  }

  return ret;
}


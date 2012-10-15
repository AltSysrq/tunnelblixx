#ifndef AUDIO_SOURCE_HXX_
#define AUDIO_SOURCE_HXX_

#include <SDL.h>

/**
 * Describes a generic source of audio data.
 */
class AudioSource {
public:
  virtual ~AudioSource() {}

  /**
   * Retrieves the next count sample-points from the audio stream.
   *
   * Buffer is to be filled with count/2 samples in the following format:
   *   signed 16-bit machine-endian stereo 44100 Hz LPCM
   *
   * Returns the number of sample-points which were actually written to the
   * buffer; returning -1 indicates that the AudioSource has completed and will
   * never return more data.
   */
  virtual signed getAudio(Sint16* dst, unsigned count) = 0;
};

#endif /* AUDIO_SOURCE_HXX_ */

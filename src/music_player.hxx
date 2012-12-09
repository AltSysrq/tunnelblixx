#ifndef MUSIC_PLAYER_HXX_
#define MUSIC_PLAYER_HXX_

#include <SDL.h>

#include <vector>

#include "audio_source.hxx"

/**
 * Decodes a particular type of sound stream to the format required by the
 * Mixer.
 */
class MusicDecoder {
protected:
  /**
   * A temporary buffer for when resampling is required.
   */
  std::vector<Sint16> tempBuff;

public:
  virtual ~MusicDecoder() {};

  /**
   * Decodes the next len sample-points.
   *
   * Returns the number of sample-points decoded, or -1 to indicate end-of-file.
   */
  virtual signed decode(Sint16* dst, unsigned len) = 0;

protected:
  /**
   * Up- or down-selects channels from the temporary buffer into the given
   * buffer.
   *
   * @param dst The buffer into which to write
   * @param lc Channel offset of left channel
   * @param rc Channel offset of right channel
   * @param nc The total number of channels
   * @param len The number of relevant entries in tempBuff
   * @return The number of sample points in dst
   */
  unsigned select(Sint16* dst, unsigned lc, unsigned rc, unsigned nc,
                  unsigned len) const;

  /**
   * In-place re-samples the given data (which must already conform to the
   * expected audio format other than in sample rate) from the given sample
   * rate to the expected sample-rate.
   *
   * @param buff The buffer to modify
   * @param len The number of sample-points currently in buffer
   * @param maxlen The maximum size to allow the output buffer to be; must
   * represent a whole number of samples
   * @param rate The current sample rate of the buffer
   * @return The number of sample-points in the final buffer
   */
  unsigned resample(Sint16* buff, unsigned len, unsigned maxlen,
                    unsigned rate) const;

  /**
   * Ensures tempBuff has space sufficient to obtain len/2 output samples given
   * the channels per sample, and the input sample rate, then returns the
   * number of sample-points to read from the unreformatted input.
   */
  unsigned ensureCapacity(unsigned len, unsigned channels, unsigned rate);
};

/**
 * Plays music files, rotating through the list given, and tracking the
 * amplitude of each section read.
 */
class MusicPlayer: public AudioSource {
  MusicDecoder* decoder;
  void (*amplitudeCallback)(void*, float);
  void* callbackUserdata;

  const char*const*const files;
  const unsigned numFiles;
  unsigned fileIx;

public:
  /**
   * Constructs a MusicPlayer with the given list of input files and amplitude
   * callback.
   *
   * The callback will be called at the end of each getAudio() call to indicate
   * the current amplitude of the stream.
   */
  MusicPlayer(const char*const*, unsigned,
              void (*callback)(void* ud, float), void* ud);
  virtual ~MusicPlayer();

  virtual signed getAudio(Sint16*, unsigned);
};

#endif /* MUSIC_PLAYER_HXX_ */

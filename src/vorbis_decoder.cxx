#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "vorbis_decoder.hxx"
#include "music_player.hxx"

using namespace std;

class VorbisDecoder: public MusicDecoder {
  OggVorbis_File decoder;

public:
  VorbisDecoder(const char*);
  virtual ~VorbisDecoder();
  virtual signed decode(Sint16*, unsigned);

private:
  static unsigned leftChannel(const vorbis_info*);
  static unsigned rightChannel(const vorbis_info*);
};

MusicDecoder* openVorbisDecoder(const char* path) {
  try {
    return new VorbisDecoder(path);
  } catch (...) {
    return NULL;
  }
}

VorbisDecoder::VorbisDecoder(const char* path) {
  errno = 0;

  if (int ret = ov_fopen(path, &decoder)) {
    cerr << "Unable to open " << path << " for decoding: ";
    if (errno)
      cerr << strerror(errno);
    else
      switch (ret) {
      case OV_EREAD: cerr << "Unable to read from file";
      case OV_ENOTVORBIS: cerr << "Not a valid vorbis file";
      case OV_EVERSION: cerr << "Bad vorbis version";
      case OV_EBADHEADER: cerr << "Corrupt vorbis headers";
      case OV_EFAULT: cerr << "Internal logic error";
      default: cerr << "Unknown (code " << ret << ")";
      }
    cerr << endl;

    /* Just throw something since we can't fulfil the contract.
     * openVorbisDecoder() just catches everything, so this value doesn't
     * matter.
     */
    throw 0;
  }
}

VorbisDecoder::~VorbisDecoder() {
  ov_clear(&decoder);
}

signed VorbisDecoder::decode(Sint16* dst, unsigned maxlen) {
  union {
    Sint16 one;
    char bytes[2];
  } detectEndianness;
  detectEndianness.one = 1;
  bool isLittleEndian = !!detectEndianness.bytes[0];

  /* Vorbis will not necessarily return as much data as we want, and the format
   * may change between calls to ov_read(), so we need to retrieve the info for
   * each sub-read info operate on and transform it into the destination before
   * reading the next portion.
   */
  unsigned nread = 0;
  while (maxlen) {
    int currentStream;
    const vorbis_info* info = ov_info(&decoder, -1);
    unsigned maxSamplePoints = ensureCapacity(maxlen, info->channels, info->rate);

    int bytesRead = ov_read(&decoder, (char*)&tempBuff[0], maxSamplePoints * 2,
                            isLittleEndian? 0 : 1, 2, 1, &currentStream);
    if (!bytesRead)
      break;

    unsigned samplePointsRead = bytesRead / 2;
    /* Downselect (or upselect) to stereo */
    ensureCapacity(maxlen, 2, info->rate);
    samplePointsRead = select(&tempBuff[0],
                              leftChannel(info), rightChannel(info),
                              info->channels, samplePointsRead);
    /* Resample to correct sampling frequency */
    ensureCapacity(maxlen, 2, AUDIO_SAMPLE_RATE);
    samplePointsRead = resample(&tempBuff[0], samplePointsRead, maxlen,
                                info->rate);
    /* Copy to destination and advance */
    memcpy(dst, &tempBuff[0], samplePointsRead * 2);
    dst += samplePointsRead;
    maxlen -= samplePointsRead;
  }

  if (nread)
    return nread;
  else
    /* EOF */
    return -1;
}

static const unsigned stdLeftChannels[8] = {
  0, /* mono - only channel */
  0, /* stereo - left */
  0, /* 1d-surround - left */
  0, /* quadraphonic surround - front left */
  0, /* five-channel surround - front left */
  0, /* 5.1 surround - front left */
  3, /* 6.1 surround - side left */
  3, /* 7.1 surround - side left */
};
static const unsigned stdRightChannels[8] = {
  0, /* mono - only channel */
  1, /* stereo - right */
  2, /* 1d-surround - right */
  1, /* quadraphonic surround - front right */
  2, /* five-channel surround - front right */
  2, /* 5.1 surround - front right */
  4, /* 6.1 surround - side right */
  4, /* 7.1 surround - side right */
};
    
unsigned VorbisDecoder::leftChannel(const vorbis_info* info) {
  if (info->channels <= 8)
    return stdLeftChannels[info->channels-1];
  else
    /* No clue, take the first stream and hope for the best */
    return 0;
}

unsigned VorbisDecoder::rightChannel(const vorbis_info* info) {
  if (info->channels <= 8)
    return stdRightChannels[info->channels-1];
  else
    /* No clue, take the first stream and hope for the best */
    return 0;
}

AM_CXXFLAGS=$(SDL_CFLAGS) $(SDLIMG_CFLAGS) $(GL_CFLAGS) $(VORBISFILE_CFLAGS) \
  -std=c++11
AM_LDFLAGS=$(SDL_LIBS) $(SDLIMG_LIBS) $(GL_LIBS) $(VORBISFILE_LIBS)
bin_PROGRAMS=tunnelblick
tunnelblick_SOURCES= \
  tunnelblick.cxx \
  game.cxx \
  tunnel.cxx \
  distortion.cxx \
  globals.cxx

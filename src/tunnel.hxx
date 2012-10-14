#ifndef TUNNEL_HXX_
#define TUNNEL_HXX_

#include <list>
#include <queue>

class Distortion;

/**
 * Creates a visual grid representing the path along the virtual Z axis. The
 * path and walls are a grid coloured by moving "pulses".
 */
class Tunnel {
public:
  static const unsigned gridWidth = 4;
  static const unsigned gridLength = 256;
  static const float gsqsz = 0.25f; //1.0f / gridWidth
  static const float gsqlen = 0.5f; //gsqsz / 2.0f

private:
  float grid[gridLength][gridWidth][3];
  struct Pulse {
    float coord, speed, dr, dg, db;
  };
  std::list<Pulse> pulses[4];
  static const unsigned maxPulses = 1024;

  struct QueuedPulse {
    unsigned column;
    float when;
    Pulse pulse;

    bool operator==(const QueuedPulse& that) const {
      return when == that.when;
    }
    bool operator<(const QueuedPulse& that) const {
      return when < that.when;
    }
  };

  std::priority_queue<QueuedPulse> pulseQueue;

  float offset, clock;

public:
  Tunnel();
  /**
   * Updates the Tunnel, given the elapsed time in seconds.
   */
  void update(float);
  /** Draws the Tunnel. */
  void draw(const Distortion&);
  /** Queues a new pulse of the given colour and speed to appear after the
   * given time.
   */
  void pulse(float z, unsigned col,
             float r, float g, float b,
             float speed, float delay);

  /**
   * Translates the tunnel by the given amount, to keep up with the player.
   */
  void translateZ(float);
};

#endif /* TUNNEL_HXX_ */

#ifndef TUNNEL_HXX_
#define TUNNEL_HXX_

#include <list>
#include <queue>

/**
 * Creates a visual grid representing the path along the virtual Z axis. The
 * path and walls are a grid coloured by moving "pulses".
 */
class Tunnel {
public:
  static constexpr unsigned gridWidth = 4;
  static constexpr unsigned gridLength = 256;
  static constexpr float gsqsz = 1.0f / gridWidth;
  static constexpr float gsqlen = gsqsz*2;

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
  void draw();
  /** Queues a new pulse of the given colour and speed to appear after the
   * given time.
   */
  void pulse(float z, unsigned col,
             float r, float g, float b,
             float speed, float delay);
};

#endif /* TUNNEL_HXX_ */

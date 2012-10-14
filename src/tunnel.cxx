#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include <list>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "tunnel.hxx"

using namespace std;

Tunnel::Tunnel() {
  //Create initial pulses
  for (unsigned off = 0; off < gridLength; off += 32) {
    for (unsigned i = 0; i < gridWidth; ++i) {
      pulse(off*gsqlen, i, +0.5f, +0.5f, +0.5f, +5.0f, 0);
      pulse(off*gsqlen, i, -0.5f, -0.5f, -0.5f, -5.0f, 0);
    }
  }
}

void Tunnel::update(float et) {
  clock += et;

  //Dequeue pulses whose time has come
  while (!pulseQueue.empty() && pulseQueue.top().when < clock) {
    const QueuedPulse& qp = pulseQueue.top();
    pulses[qp.column].push_back(qp.pulse);

    if (pulses[qp.column].size() > maxPulses)
      pulses[qp.column].pop_front();

    pulseQueue.pop();
  }

  //Update pulses
  for (list<Pulse>& llp: pulses) {
    for (Pulse& p: llp) {
      p.coord += p.speed * et;
      while (p.coord < 0)           p.coord += gridLength;
      while (p.coord >= gridLength) p.coord -= gridLength;
    }
  }
}

void Tunnel::draw() {
  //Reset grid to neutral
  for (unsigned i = 0; i < gridLength; ++i)
    for (unsigned j = 0; j < gridWidth; ++j)
      for (unsigned k = 0; k < 3; ++k)
        grid[i][j][k] = 0.5f;

  //Add in pulses
  for (unsigned col = 0; col < gridWidth; ++col) {
    for (const Pulse& p: pulses[col]) {
      unsigned lower = (unsigned)floor(p.coord);
      unsigned upper = (unsigned)ceil(p.coord);
      if (upper == gridLength)
        upper = 0;

      float uw = p.coord - lower;
      float lw = 1.0f - uw;

      grid[lower][col][0] += lw*p.dr;
      grid[upper][col][0] += uw*p.dr;
      grid[lower][col][1] += lw*p.dg;
      grid[upper][col][1] += uw*p.dg;
      grid[lower][col][2] += lw*p.db;
      grid[upper][col][2] += uw*p.db;
    }
  }

  //Translate for partial squares
  float zt = (floor(offset)-offset)*gsqlen;

  glBegin(GL_TRIANGLES);

  //Draw the floor
  signed firstFloorTile = (signed)floor(offset);
  if (firstFloorTile < 0)
    firstFloorTile += gridLength;
  const float halfSpace = gsqsz*0.05f;
  for (unsigned z = 0; z < gridLength/2; ++z) {
    for (unsigned x = 0; x < gridWidth; ++x) {
      glColor3fv(grid[(firstFloorTile+z) % gridLength][x]);
      glVertex3f((x+0)*gsqsz + halfSpace, 0, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f((x+1)*gsqsz - halfSpace, 0, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f((x+0)*gsqsz + halfSpace, 0, -((z+1)*gsqlen - halfSpace + zt));

      glVertex3f((x+1)*gsqsz - halfSpace, 0, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f((x+0)*gsqsz + halfSpace, 0, -((z+1)*gsqlen - halfSpace + zt));
      glVertex3f((x+1)*gsqsz - halfSpace, 0, -((z+1)*gsqlen - halfSpace + zt));
    }
  }

  //Draw the walls
  unsigned firstWallTile = firstFloorTile + gridLength / 2;
  for (unsigned z = 0; z < gridLength / 2; ++z) {
    for (unsigned i = 0; i < gridWidth; ++i) {
      glColor3fv(grid[(firstWallTile + (gridLength/2-z-1)) % gridLength][i]);
      float x = (i < gridWidth / 2? 0 : 1);
      float y = 2*(i < gridWidth / 2? i*gsqsz : (gridWidth-i-1)*gsqsz);
      glVertex3f(x, y +   0.0f*2 + halfSpace, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f(x, y + gsqsz*2 - halfSpace, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f(x, y +   0.0f*2 + halfSpace, -((z+1)*gsqlen - halfSpace + zt));

      glVertex3f(x, y + gsqsz*2 - halfSpace, -((z+0)*gsqlen + halfSpace + zt));
      glVertex3f(x, y +   0.0f*2 + halfSpace, -((z+1)*gsqlen - halfSpace + zt));
      glVertex3f(x, y + gsqsz*2 - halfSpace, -((z+1)*gsqlen - halfSpace + zt));
    }
  }

  glEnd();
}

void Tunnel::pulse(float z, unsigned col,
                   float r, float g, float b,
                   float speed, float delay) {
  Pulse pulse = { -z/gsqlen + offset, speed, r, g, b };
  QueuedPulse qp = { col, clock + delay, pulse };
  pulseQueue.push(qp);
}

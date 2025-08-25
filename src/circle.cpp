#include "circle.h"
#include <cmath>

float Circle::distanceTo(const Circle& other) const {
  float dx = x - other.x;
  float dy = y - other.y;
  return std::sqrt(dx * dx + dy * dy);
}

void Circle::applyCollision(Circle& other) {
  float dist = distanceTo(other);
  float minDist = radius + other.radius;
  float dx = x - other.x;
  float dy = y - other.y;

  if (dist < minDist && dist > 0.0f) {
    float nx = dx / dist;
    float ny = dy / dist;
    float vxRel = vx - other.vx;
    float vyRel = vy - other.vy;

    float relDot = vxRel * nx + vyRel * ny; // Dot product relative velocity and collision normal

    if (relDot > 0) return;
    float m1 = mass;
    float m2 = other.mass;
    float impulse = (2 * relDot) / (1/m1 + 1/m2);
    vx -= (impulse / m1) * nx;
    vy -= (impulse / m1) * ny;

    other.vx += (impulse / m2) * nx;
    other.vy += (impulse / m2) * ny;

    // Overlap fixer
    float overlap = 0.5f * (minDist - dist);
    x += nx * overlap;
    y += ny * overlap;
    other.x -= nx * overlap;
    other.y -= ny * overlap;
  }
}

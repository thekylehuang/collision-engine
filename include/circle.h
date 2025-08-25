#pragma once
#include <vector>

struct Circle {
  float x, y;
  float vx, vy;
  float radius;
  float mass;

  Circle(float x, float y, float vx, float vy, float radius, float mass) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this-> vy = vy;
    this-> radius = radius;
    this-> mass = mass;
  }

  float distanceTo(const Circle& other) const;
  void updateRK4(float dt, const std::vector<Circle>& others);
  void applyCollision(Circle& other);
};
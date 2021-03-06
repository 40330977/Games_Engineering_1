#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;

  bool isGrounded() const;

public:
  void update(double dt) override;
  void changeSize(const sf::Vector2f& size);
  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;

  //void Sizer(Vector2f sizes);
};

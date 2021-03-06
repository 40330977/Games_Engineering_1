#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;
using namespace Physics;





bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {
	

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::Right)) {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (getVelocity().x < _maxVelocity.x)
        impulse({(float)(dt * _groundspeed), 0});
    } else {
      if (getVelocity().x > -_maxVelocity.x)
        impulse({-(float)(dt * _groundspeed), 0});
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }

  // Handle Jump
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    _grounded = isGrounded();
    if (_grounded) {
      setVelocity(Vector2f(getVelocity().x, 0.f));
      teleport(Vector2f(pos.x, pos.y - 2.0f));
	  if (Keyboard::isKeyPressed(Keyboard::D))
	  {
		  impulse(Vector2f(0, -14.f));
	  }
	  else { 
		  impulse(Vector2f(0, -7.f));
	  }
    }
  }
  

  //Are we in air?
  if (!_grounded) {
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.1f);
  }
 

  if (Keyboard::isKeyPressed(Keyboard::S))
	{
	  
		_maxVelocity = Vector2f(600.f, 800.f);
		_groundspeed = 160.0f;
		
	}
  else 
  {
	  _maxVelocity = Vector2f(200.f, 400.f);
	  _groundspeed = 30.f;
  }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  bool isbigger = false;
  if (Keyboard::isKeyPressed(Keyboard::A) && isbigger == false)
  {
	  //_size *= 2.0;
	  isbigger = true;
  }

  PhysicsComponent::update(dt);
}

void PlayerPhysicsComponent::changeSize(const Vector2f & size)
{
	b2FixtureDef FixtureDef;
	FixtureDef.friction = 0.1f;
	FixtureDef.restitution = .2;
	b2PolygonShape Shape;
	Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
	FixtureDef.shape = &Shape;
	_body->DestroyFixture(_fixture);
	_fixture = _body->CreateFixture(&FixtureDef);
	_size = sv2_to_bv2(size, true);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}

//void PlayerPhysicsComponent::Sizer(Vector2f sizes)
//{
//	_size = sv2_to_bv2(sizes, true);
//}

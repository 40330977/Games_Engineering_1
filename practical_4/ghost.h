//ghost.h
#pragma once
#include "entity.h"

class Ghost : public Entity {
private:
	float _speed;

public:
	void update(double dt) override;
	Ghost();
	void render(sf::RenderWindow &window) const override;
};
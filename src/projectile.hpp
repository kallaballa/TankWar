/*
 * projectile.hpp
 *
 *  Created on: Mar 1, 2014
 *      Author: elchaschab
 */

#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include "object.hpp"
#include "params.hpp"
#include <limits>

namespace tankwar {

class Tank;

class Projectile : public Object {
public:
	Tank* owner_;
	Object* nearestObject_;
	Vector2D nearestEnemyLoc_;
	Coord nearestEnemyDis_;
	Vector2D nearestFriendLoc_;
	Coord nearestFriendDis_;
	Vector2D startLoc_;
	bool enemyHitter_;
	bool friendHitter_;

	Projectile(Tank& owner, Vector2D& loc, Coord rotation) :
		Object(loc, rotation, Params::PROJECTILE_RANGE, Params::MAX_PROJECTILE_SPEED, false, false),
		owner_(&owner),
		nearestObject_(NULL),
		nearestEnemyLoc_(std::numeric_limits<Coord>().max(),std::numeric_limits<Coord>().max()),
		nearestEnemyDis_(std::numeric_limits<Coord>().max()),
		nearestFriendLoc_(std::numeric_limits<Coord>().max(),std::numeric_limits<Coord>().max()),
		nearestFriendDis_(std::numeric_limits<Coord>().max()),
		startLoc_(loc),
		enemyHitter_(false),
		friendHitter_(false){
		dir_ = directionFromRotation(rotation);
	}

	void move() {
		//update location
		loc_ += (dir_ * speed_);
	}

	ObjectType type() {
		return ObjectType::PROJECTILE;
	}
};

}

#endif /* PROJECTILE_HPP_ */

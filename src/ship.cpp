#include "ship.hpp"
#include "battlefield.hpp"
#include "population.hpp"
#include "../src/lua.hpp"
#include <cstdlib>
#include <iostream>
#include "util.hpp"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

namespace neurocid {

Ship::Ship(size_t teamID, ShipLayout tl, Brain* brain) :
		Object(SHIP, {0,0}, 0, tl.radius_, false, false, false),
		teamID_(teamID),
		layout_(tl),
		brain_(brain),
		scan_(this),
		ammo_(tl.startAmmo_){
	resetGameState();
}

void Ship::setBrain(Brain* b) {
	assert(brain_ == NULL);
	brain_ = b;
}

static const Coord euler_constanct = std::exp(1.0);
inline Coord sigmoid(const Coord& x) {
	return (1 / (1 + pow(euler_constanct,-x)));
}

void Ship::calculateFitness(const BattleFieldLayout& bfl) {
	fitness_ = neurocid::lua::run_fitness_function(layout_.fitnessFunction_,*this);
}

void Ship::think(BattleFieldLayout& bfl) {
	if(layout_.isDummy_)
		return;

	assert(brain_ != NULL);
	brain_->update(bfl, this->scan_);
	brain_->run();
}

void Ship::move(BattleFieldLayout& bfl) {
	if(layout_.isDummy_)
		return;

	assert(brain_ != NULL);
	//assign the outputs
	flthrust_ = brain_->lthrust_;
	frthrust_ = brain_->rthrust_;
	blthrust_ = brain_->fthrust_;
	brthrust_ = brain_->bthrust_;

	bool canShoot = layout_.canShoot_ && (cool_down == 0 && ammo_ > 0);
	bool wantsShoot = (brain_->shoot_ > 0.0);
	if(canShoot && wantsShoot) {
		willShoot_ = true;
	} else if(cool_down > 0){
		if(wantsShoot)
			++failedShots_;

		willShoot_ = false;
		--cool_down;
	}

	//std::cerr << "canMove: " << tl_.canMove_ << "\tcanRotate: " << tl_.canRotate_ << "\tspeed: " << speed_ << "\trotForce:" << rotForce_  << std::endl;
}

void Ship::damage() {
	damage_++;
	if (damage_ >= layout_.maxDamage_) {
		death();
	}
}

void Ship::death() {
	damage_ = layout_.maxDamage_;
	dead_ = true;
	explode_ = true;
}

// demote and execute this unit
void Ship::kill() {
	projectiles_.clear();
	scan_.objects_.clear();
	hits_ = 0;
	recharged_ = 0;
	fuel_ = 0;
	death();
}

void Ship::crash() {
	crash_++;
	crashDamage_++;
	crashed_ = true;

	if(crashDamage_ >= layout_.crashesPerDamage_) {
		crashDamage_ = 0;
		damage();
	}
}

void Ship::impact(Ship& other) {
	crash();
	other.crash();
}

void Ship::impact(Projectile& p) {
	p.death();
	damage();

	if (p.owner_->teamID_ != teamID_) {
		p.owner_->hits_++;
	} else {
		p.owner_->friendlyFire_++;
	}
}

void Ship::recharge() {
	Coord amount = (layout_.maxFuel_ - fuel_);
	assert(amount >= 0);
	recharged_ += amount;
	fuel_ = layout_.maxFuel_;
	ammo_ = layout_.maxAmmo_;
}

void Ship::capture() {
	++captured_;
}

Ship Ship::makeChild() const {
	assert(brain_ != NULL);
	Ship child(teamID_, layout_);
	Brain* fresh  = new Brain();
	fresh->initialize(brain_->layout_);
	child.setBrain(fresh);
	return child;
}

Ship Ship::clone() const {
	assert(brain_ != NULL);
	Ship child(teamID_, layout_);
	Brain* fresh  = new Brain();
	fresh->initialize(brain_->layout_);
	child.setBrain(fresh);

	//copy brain
	for(size_t b = 0; b < brain_->layout_.numBrains_ + 1; ++b) {
		for(size_t i = 0; i < brain_->size(b); ++i) {
			child.brain_->weights(b)[i] = brain_->weights(b)[i];
		}
	}
	return child;
}

void Ship::resetGameState() {
	for(Projectile* p : projectiles_) {
		delete p;
	}
	projectiles_.clear();
	flthrust_ = 0;
	frthrust_ = 0;
	blthrust_ = 0;
	brthrust_ = 0;
	fuel_ = layout_.startFuel_;
	ammo_ = layout_.startAmmo_;
	dead_ = false;
	explode_ = false;
	cool_down = 0;
	willShoot_ = false;
	crashed_ = false;
}

void Ship::resetScore() {
	friendlyFire_ = 0;
	crash_ = 0;
	crashDamage_ = 0;
	hits_ = 0;
	damage_ = 0;
	fitness_ = 0;
	recharged_ = 0;
	failedShots_ = 0;
	captured_ = 0;
}

void Ship::update(ShipLayout tl) {
	this->layout_ = tl;
	resetGameState();
}

bool Ship::operator==(const Ship& other) const {
	return this->loc_ == other.loc_ && this->teamID_ == other.teamID_;
}

bool Ship::operator!=(const Ship& other) const {
	return !this->operator ==(other);
}

bool Ship::operator<(const Ship& other) const {
	return (this->fitness_ < other.fitness_);
}

bool Ship::willShoot() {
	return willShoot_;
}

Projectile* Ship::shoot() {
	assert(cool_down == 0);
	assert(ammo_ > 0);
	--ammo_;

	cool_down = layout_.maxCooldown_;
	Vector2D loc = loc_;
	loc += (getDirection() * radius_);
	Projectile* p = new Projectile(*this,layout_.pl_, loc, rotation_);
	projectiles_.push_back(p);
	return p;
}

} /* namespace neurocid */

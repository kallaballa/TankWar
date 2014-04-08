#include "brain.hpp"
#include "battlefield.hpp"
#include "util.hpp"
#include <fann.h>
#include <iostream>
#include <assert.h>
#include <limits>
#include <cmath>
#include <map>


namespace neurocid {
using std::map;

void BrainSwarm::update(const BattleFieldLayout& bfl, const Scan& scan) {
	parentBrain_t::update(bfl, scan);

	map<Coord, ScanObject> friendObj;
	map<Coord, ScanObject> enemyObj;

	//sort the friendly and enemy scan objects by their angular distance
	for (const ScanObject& so : scan.objects_) {
		assert(so.type_ != INVALID);
		if(so.type_ == FRIEND) {
			Coord angDist = so.angDist_;
			auto it = friendObj.find(angDist);
			while(it != friendObj.end()) {
				if(angDist > 0)
					angDist -= fRand(0.00000001, 0.00009);
				else
					angDist += fRand(0.00000001, 0.00009);

				it = friendObj.find(angDist);
			}

			friendObj[angDist] = so;
		}
	}

	for (const ScanObject& so : scan.objects_) {
		assert(so.type_ != INVALID);
		if(so.type_ == ENEMY) {
			Coord angDist = so.angDist_;
			assert(angDist != NO_COORD);
			auto it = enemyObj.find(angDist);
			while(it != enemyObj.end()) {
				if(angDist > 0)
					angDist -= fRand(0.00000001, 0.00009);
				else
					angDist += fRand(0.00000001, 0.00009);

				it = enemyObj.find(angDist);
			}

			enemyObj[angDist] = so;
		}
	}

	assert(enemyObj.size() + friendObj.size() == scan.objects_.size());

	size_t inputCnt = 0;
	for (auto it : friendObj) {
		ScanObject& so = it.second;
		if (so.vector_ != NO_VECTOR2D) {
			applyInput(inputCnt * 2, so.vector_.x_);
			applyInput(inputCnt * 2 + 1, so.vector_.y_);
		}
		++inputCnt;
	}

	for (auto it : enemyObj) {
		ScanObject& so = it.second;
		if (so.vector_ != NO_VECTOR2D) {
			applyInput(inputCnt * 2, so.vector_.x_);
			applyInput(inputCnt * 2 + 1, so.vector_.y_);
		}
		++inputCnt;
	}

	Vector2D vel = scan.vel_;
	vel.normalize();
	applyInput((inputCnt * 2), vel.x_);
	applyInput((inputCnt * 2) + 1, vel.y_);
	++inputCnt;

	Coord angVel = scan.angVel_;
	if(angVel > 10)
		angVel = 10;
	else if(angVel < -10)
		angVel = -10;

	applyInput((inputCnt * 2), angVel / 10);
	++inputCnt;

	for(size_t i = 0; i < layout_.numInputs_; ++i) {
		assert(!std::isinf(inputs_[i]));
		assert(!std::isnan(inputs_[i]));
		assert(inputs_[i] != std::numeric_limits<fann_type>().max());
	}

	//	std::cerr << "output:\t" << lthrust_ << "\t" << rthrust_ << "\t" << shoot_ << std::endl;
}
} /* namespace neurocid */

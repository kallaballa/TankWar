/*
 * scanner1.hpp
 *
 *  Created on: Mar 14, 2014
 *      Author: elchaschab
 */

#ifndef SCANNER1_HPP_
#define SCANNER1_HPP_

#include <limits>
#include <vector>
#include "2d.hpp"
#include "bsp.hpp"
#include "scan.hpp"
#include "KMlocal.h"

namespace neurocid {
using std::numeric_limits;
using std::vector;

class BattleField;
class Population;
class Ship;

struct ScannerLayout {
	bool disableClusterCenters_;
	size_t numClusters_;
	size_t numFriends_;
	size_t numEnemies_;
	size_t numFriendFacilities_;
	size_t numEnemyFacilities_;
	size_t numProjectiles_;
};

class ScannerImpl {
public:
	ScannerLayout layout_;
	ScannerImpl(ScannerLayout& layout): layout_(layout) {}
	virtual void scan(BattleField& field) = 0;
	virtual void prepare(BattleField& field) = 0;
};

class BspScanner : public ScannerImpl {
public:
	ObjectBsp bspA_;
	ObjectBsp bspB_;
	ObjectBsp bspPA_;
	ObjectBsp bspPB_;
	ObjectBsp bspFA_;
	ObjectBsp bspFB_;

	BspScanner(ScannerLayout& layout) : ScannerImpl(layout) {};
	void findInRange(ObjectBsp& bsp, Object& from, vector<Object*>& result, size_t range);
	void findInRange(ObjectBsp& bsp, Object& from, ScanObjectType type, ScanObjectVector& result, size_t range);
	bool findNearest(ObjectBsp& bsp, Object& from, ScanObjectType type, ScanObjectVector& result);
	size_t findNearestN(ObjectBsp& bsp, Object& from, ScanObjectType type, ScanObjectVector& result, size_t num);
	std::pair<Object*,Coord> findNearest(ObjectBsp& bsp, Object& from);
	void buildBsps(BattleField& field);

	virtual void scan(BattleField& field) = 0;
	virtual void prepare(BattleField& field);
};

class SwarmScanner : public BspScanner {
private:
	void pickRandomN(ScanObjectType type, Ship& t, Population& team, ScanObjectVector& result, size_t n);
	void teamScan(Population& active, Population& passive, ObjectBsp& bspFriends, ObjectBsp& bspEnemies, BattleFieldLayout& bfl);
public:
	SwarmScanner(ScannerLayout& layout) : BspScanner(layout) {};

	virtual void scan(BattleField& field);
	virtual void prepare(BattleField& field);
};

class ClusterScanner : public ScannerImpl {
public:
	vector<Vector2D> centersA_;
	vector<Vector2D> centersB_;

	pair<Vector2D, Coord> findNearestCenter(const vector<Vector2D>& centers, const Vector2D& loc);
	void scanClusterCenters(Population& team, vector<Vector2D>& result, size_t numCenters);
	ClusterScanner(ScannerLayout& layout) : ScannerImpl(layout) {};

	virtual void scan(BattleField& field) = 0;
	virtual void prepare(BattleField& field);
};

class HybridScanner : public ClusterScanner, public BspScanner {
private:
	void teamScan(Population& active, Population& passive, vector<Vector2D>& ctrFriends, vector<Vector2D>& ctrEnemies, ObjectBsp& bspFriends, ObjectBsp& bspEnemies, ObjectBsp& bspFriendFacilities, ObjectBsp& bspEnemyFacilities, ObjectBsp& bspEnemyProjectiles, BattleFieldLayout& bfl);
public:
	HybridScanner(ScannerLayout& layout) : ClusterScanner(layout), BspScanner(layout) {};

	virtual void scan(BattleField& field);
	virtual void prepare(BattleField& field);
};

typedef HybridScanner Scanner;

} /* namespace neurocid */

#endif /* SCANNER1_HPP_ */

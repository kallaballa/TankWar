/*
 * population.hpp
 *
 *  Created on: Mar 1, 2014
 *      Author: elchaschab
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <vector>
#include <iostream>
#include "tank.hpp"

namespace tankwar {
using std::vector;
using std::istream;
using std::ostream;

struct PopulationLayout {
	friend class boost::serialization::access;
	TankLayout tl_;
	BrainLayout bl_;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	  ar & tl_;
	  ar & bl_;
	}
};

class Population: public vector<Tank> {
	  friend class boost::serialization::access;
public:
	struct Statistics {
		Statistics() {
			generationCnt_ = 0;
			reset();
		}

		//total fitness of population
		double totalFitness_;
		//best fitness this population
		double bestFitness_;
		//average fitness
		double averageFitness_;
		//worst
		double worstFitness_;
		//keeps track of the best genome
		//generation counter
		double totalFriendlyFire_;
		double totalHits_;
		double totalDamage_;
		double totalAmmonition_;
		double averageFriendlyFire_;
		double averageHits_;
		double averageDamage_;
		double averageAmmonition_;

		size_t generationCnt_;
		double score_;

		size_t fittestGenome_;

		void reset() {
			totalFitness_ = 0;
			bestFitness_ = 0;
			worstFitness_ = std::numeric_limits<double>().max();
			averageFitness_ = 0;
			averageFriendlyFire_ = 0;
			averageDamage_ = 0;
			averageAmmonition_ = 0;
			totalFitness_ = 0;
			totalFriendlyFire_ = 0;
			totalHits_ = 0;
			totalDamage_ = 0;
			totalAmmonition_ = 0;
			score_ = 0;
		}

		void print(std::ostream& os) {
			os << generationCnt_ << ":" << bestFitness_ << ":"
					<< averageFitness_ << ":" << averageHits_ << ":"
					<< averageFriendlyFire_ / 3 << ":" << averageHits_ << ":"
					<< averageDamage_ << ":" << (averageAmmonition_ / 3) << ":"
					<< (score_);
		}
	};

	PopulationLayout layout_;
	size_t score_ = 0;
	bool winner_ = false;
	Statistics stats_;

	void update(TankLayout tl) {
		for (Tank& t : *this) {
			t.update(tl);
		}
	}

	size_t countDead() {
		size_t cnt = 0;
		for (Tank& t : *this) {
			if (t.dead_)
				cnt++;
		}
		return cnt;
	}

	Population& operator=(const Population& other) {
		layout_ = other.layout_;
		score_ = other.score_;
		winner_ = other.winner_;
		stats_ = other.stats_;
		vector<Tank>::operator=(other);
		return *this;
	}

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	  ar & layout_;
	  ar & score_;
	  ar & winner_;
	  ar & *((vector<Tank>*)this);
	}
};


inline void read_teams(vector<Population>& teams, istream& is) {
  boost::archive::text_iarchive ia(is);
  ia >> teams;
}

inline void write_teams(vector<Population>& teams, ostream& os) {
  boost::archive::text_oarchive oa(os);
  oa << teams;
}
}

#endif /* POPULATION_HPP_ */

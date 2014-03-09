/*
 * game.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: elchaschab
 */

#include "game.hpp"
#include "gamestate.hpp"
#include "renderer.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

namespace tankwar {

Game::Game(size_t battleIterations, vector<Population>& teams, vector<GeneticPool>& pools, Placer& placer) :
	battleIterations_(battleIterations),
	teams_(teams),
	newTeams_(teams.size()),
	pools_(pools),
	placer_(placer){
}

void Game::prepare() {
	//shuffle positions
	for(Population& team : teams_) {
		random_shuffle(team.begin(), team.end());
		for(Tank& t : team) {
			t.resetScore();
		}
	}
}

void Game::place() {
	Options& opts = *Options::getInstance();
	placer_.place(teams_, Vector2D(opts.WINDOW_WIDTH / 2, opts.WINDOW_HEIGHT / 2), std::min(opts.WINDOW_WIDTH, opts.WINDOW_HEIGHT) - 300, 20);
}

void Game::fight() {
	BattleField field(teams_);
	GameState& gs = *GameState::getInstance();
	for(size_t i = 0; (i < battleIterations_) && GameState::getInstance()->isRunning(); ++i) {
		field.step();
		if(gs.isSlow())
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		else if(gs.isSlower())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		Renderer::getInstance()->update(&field);
	}
	Renderer::getInstance()->update(NULL);
}

void Game::score() {
	vector<size_t> alive(2,0);

	for(size_t i = 0; i < teams_.size(); ++i) {
		Population& team = teams_[i];
		for(Tank& t : team) {
			if(!t.dead_)
				++alive[i];
		}
	}
	assert(teams_.size() == 2);

	if(alive[0] != alive[1]) {
		if(alive[0] > alive[1]) {
			teams_[0].score_++;
			teams_[0].winner_=true;
		} else {
			teams_[1].score_++;
			teams_[1].winner_=true;
		}
	}
}

void Game::mate() {
	newTeams_.clear();
	for(size_t i = 0; i < teams_.size(); ++i) {
		newTeams_.push_back(pools_[i].epoch(teams_[i]));
	}
	newTeams_[0].score_ = teams_[0].score_;
	newTeams_[1].score_ = teams_[1].score_;
}


void Game::cleanup() {
	for(Population& p : teams_) {
		for(Tank& t : p) {
			assert(!t.brain_.isDestroyed());
			t.brain_.destroy();
		}
	}
}

void Game::print() {
	for(GeneticPool& pool : pools_) {
		pool.statistics().print(std::cout);
		std::cout << ":";
	}
}

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F>
    static typename TimeT::rep execution(F const &func)
    {
        auto start = std::chrono::system_clock::now();
        func();
        auto duration = std::chrono::duration_cast< TimeT>(
            std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

vector<Population> Game::play() {
	std::cerr << 1000.0 / measure<>::execution( [&]() {
		if(GameState::getInstance()->isRunning()) prepare();
		if(GameState::getInstance()->isRunning()) place();
		if(GameState::getInstance()->isRunning()) fight();
		if(GameState::getInstance()->isRunning()) score();
		if(GameState::getInstance()->isRunning()) mate();
		cleanup();

		if(GameState::getInstance()->isRunning()) print();
	}) << std::endl;

	return newTeams_;
}
} /* namespace tankwar */

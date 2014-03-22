#include "gamestate.hpp"
#include "game.hpp"

namespace tankwar {
	GameState* GameState::instance_ = NULL;
	void GameState::setCurrentGame(Game* g) {
		currentGame_ = g;
	}

	Game* GameState::getCurrentGame() {
		return currentGame_;
	}
}

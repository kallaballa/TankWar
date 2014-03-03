
#include "canvas.hpp"
#include "tank.hpp"
#include "projectile.hpp"
#include "battlefield.hpp"
#include "population.hpp"
#include <thread>
#include <chrono>

namespace tankwar {

Canvas::Canvas(Coord width, Coord height) :
  screen_(NULL) , enabled_(true), width_(width), height_(height), timeout_(20) {
  if (width > 0 && height > 0) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
      printf("Can't init SDL:  %s\n", SDL_GetError());
      exit(1);
    }
    atexit(SDL_Quit);
    screen_ = SDL_SetVideoMode(width, height, 16, SDL_SWSURFACE);
    if (screen_ == NULL) {
      printf("Can't set video mode: %s\n", SDL_GetError());
      exit(1);
    }
  }
}

void Canvas::drawLine(Coord x0, Coord y0, Coord x1, Coord y1, Color& c) {
  if(screen_ != NULL) {
    lineRGBA(screen_, x0, y0, x1, y1, c.r, c.g, c.b, 128);
  }
}

void Canvas::drawTank(Tank& tank) {
	Color c = {255,0,255};
	if(tank.teamID_ == 0)
		c = {0,255,0};

    ellipseRGBA(screen_, (Uint16)tank.loc_.x, (Uint16)tank.loc_.y, (Uint16)tank.range_, (Uint16)tank.range_, c.r, c.g, c.b, 255);
    Vector2D tip = tank.loc_;
    tip += tank.dir_ * 5;
    ellipseRGBA(screen_, (Uint16)tip.x, (Uint16)tip.y, 3, 3, c.r, c.g, c.b, 255);
}

void Canvas::drawProjectile(Projectile& pro, Color& c) {
    ellipseRGBA(screen_, (Uint16)pro.loc_.x, (Uint16)pro.loc_.y, (Uint16)pro.range_, (Uint16)pro.range_, c.r, c.g, c.b, 255);
}

void Canvas::drawExplosion(Object& o, Color& c) {
	 ellipseRGBA(screen_, (Uint16)o.loc_.x, (Uint16)o.loc_.y, (Uint16)10, (Uint16)10, c.r, c.g, c.b, 255);
}

void Canvas::clear() {
  SDL_FillRect(screen_,NULL, 0x000000);
}

void Canvas::update() {
  if(screen_ != NULL) {
	  SDL_Flip(screen_);
  }
}

void Canvas::render(BattleField& field) {
	if(!isEnabled())
		return;
	Color white = {255,255,255};
	Color red = {255,0,0};

	this->clear();
	for(Tank& t : field.teamA_) {
		if(t.explode_)
			this->drawExplosion(t, red);
		else if(!t.dead_)
			this->drawTank(t);
		t.explode_ = false;
	}

	for(Tank& t : field.teamB_) {
		if(t.explode_)
			this->drawExplosion(t, red);
		else if(!t.dead_)
			this->drawTank(t);
		t.explode_ = false;
	}

	for(Projectile& p : field.projectiles_) {
		if(p.explode_)
			this->drawExplosion(p, red);
		else if(!p.dead_)
			this->drawProjectile(p,white);
		p.explode_ = false;
	}
	this->update();
	std::this_thread::sleep_for(std::chrono::milliseconds(timeout_));
}
}

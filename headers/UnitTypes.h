#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "MathAddon.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"
#include "Projectile.h"
#include "Timer.h"
#include "Turret.h"


class Solder: public Unit
{
public:
    Solder(SDL_Renderer* renderer, Vector2D setPos,int wave);
};

class Runner: public Unit
{
public:
    Runner(SDL_Renderer* renderer, Vector2D setPos,int wave);
};

class MiniBoss: public Unit
{
public:
    MiniBoss(SDL_Renderer* renderer, Vector2D setPos,int wave);
};

class Boss: public Unit
{
public:
    Boss(SDL_Renderer* renderer, Vector2D setPos,int wave);
};
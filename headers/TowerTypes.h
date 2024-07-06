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


class Grenadier: public Turret
{
public:
    Grenadier(SDL_Renderer* renderer, Vector2D setPos);
};

class Archer: public Turret
{
public:
    Archer(SDL_Renderer* renderer, Vector2D setPos);
};

class Wizard: public Turret
{
public:
    Wizard(SDL_Renderer* renderer, Vector2D setPos);
};


#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "MathAddon.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"
#include "Projectile.h"
#include "Timer.h"


class Turret {
public:
    Turret(SDL_Renderer *renderer, Vector2D setPos, float timer, std::string texture, float range, float degSpeed,
           int damage, bool splash, int chain, float slow, float distMax, int damagePerLVL);

    void update(SDL_Renderer *renderer, float dT, std::vector<std::shared_ptr<Unit>> &listUnits,
                std::vector<Projectile> &listProjectiles);

    void draw(SDL_Renderer *renderer, int tileSize, TTF_Font *font);

    bool checkIfOnTile(int x, int y);

    void lvlUp(int *money);

private:
    bool updateAngle(float dT);

    void shootProjectile(SDL_Renderer *renderer, std::vector<Projectile> &listProjectiles);

    void drawTextureWithOffset(SDL_Renderer *renderer, SDL_Texture *textureSelected,
                               int offset, int tileSize);

    std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>> &listUnits);


    Vector2D pos;
    float speedAngular, weaponRange, angle;
    int damage;
    bool splash;
    int chain;
    int lvl;
    float distMax;
    float slow;
    int damagePerLvl;

    Timer timerWeapon;

    std::weak_ptr<Unit> unitTarget;

    SDL_Texture *textureMain = nullptr,
            *textureShadow = nullptr;

    SDL_Surface *surfLVL;
    SDL_Texture *textLVL;

};
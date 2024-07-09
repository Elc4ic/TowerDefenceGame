#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"


class Projectile {
public:
    Projectile(SDL_Renderer *renderer, Vector2D setPos, Vector2D setDirectionNormal,int damage,bool splash,float slow,float distMax);

    void update(float dT, std::vector<std::shared_ptr<Unit>> &listUnits);

    void draw(SDL_Renderer *renderer, int tileSize);

    bool getCollisionOccurred();


private:
    void checkCollisions(std::vector<std::shared_ptr<Unit>> &listUnits);


    Vector2D pos, directionNormal;
    static const float speed, size;
    float distanceTraveledMax;
    float distanceTraveled = 0.0f;
    int damage;
    bool splash;
    float slow;

    SDL_Texture *texture = nullptr;

    bool collisionOccurred = false;
};
#pragma once

#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"
#include "Timer.h"
#include "SDL2/SDL_ttf.h"

class Unit {
public:
    Unit(SDL_Renderer *renderer, Vector2D setPos, const std::string &textureName, float speed, int hp);

    void update(float dT, Level &level, std::vector<std::shared_ptr<Unit>> &listUnits, int *target_hp);

    void draw(SDL_Renderer *renderer, int tileSize, TTF_Font *font);

    bool checkOverlap(Vector2D posOther, float sizeOther);

    bool isAlive();

    Vector2D getPos();

    void removeHealth(int damage, float slow);


private:
    Vector2D pos;
    static const float size;
    float speed;
    int hp;
    float slowed = 1.0f;

    SDL_Texture *texture = nullptr;

    Timer timerJustHurt;
    Timer timerSlowed;

    SDL_Surface *surfHPT{};
    SDL_Texture *textHP{};
};
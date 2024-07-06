#pragma once

#include <vector>
#include <chrono>
#include <memory>
#include "SDL2/SDL.h"
#include "Unit.h"
#include "Turret.h"
#include "Projectile.h"
#include "Level.h"
#include "Timer.h"


class Game {
private:
    enum class PlacementMode {
        wizard,
        archer,
        grenadier
    } placementModeCurrent;

    const int wizardCost = 120;
    const int archerCost = 70;
    const int grenadierCost = 200;


public:
    Game(SDL_Window *window, SDL_Renderer *renderer, int windowWidth, int windowHeight);

    ~Game();


private:
    void processEvents(SDL_Renderer *renderer, bool &running, int *money);

    void update(SDL_Renderer *renderer, float dT, int *target_hp, int *money, bool &running);

    void updateUnits(float dT, int *target_hp, int *money);

    void updateProjectiles(float dT);

    void updateSpawnUnitsIfRequired(SDL_Renderer *renderer, float dT);

    void draw(SDL_Renderer *renderer, int *target_hp, int *money);

    void addUnit(SDL_Renderer *renderer, Vector2D posMouse);

    void addTurret(SDL_Renderer *renderer, Vector2D posMouse, int TurretType, int *money);

    void removeTurretsAtMousePosition(Vector2D posMouse);

    int mouseDownStatus = 0;

    const int tileSize = 66;
    Level level;

    std::vector<std::shared_ptr<Unit>> listUnits;
    std::vector<Turret> listTurrets;
    std::vector<Projectile> listProjectiles;

    SDL_Texture *textureHP = nullptr;
    SDL_Texture *textureCoin = nullptr;

    Timer spawnTimer, roundTimer;
    int spawnUnitCount = 0;

};
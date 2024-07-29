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
#include "SDL2/SDL_ttf.h"


class Game {
private:
    enum class PlacementMode {
        wizard,
        archer,
        grenadier,
        froster
    } placementModeCurrent;

    const int wizardCost = 100;
    const int archerCost = 60;
    const int grenadierCost = 160;


public:
    Game(SDL_Window *window, SDL_Renderer *renderer, int windowWidth, int windowHeight);

    ~Game();


private:
    void processEvents(SDL_Renderer *renderer, bool &running);

    void update(SDL_Renderer *renderer, float dT, bool &running);

    void updateUnits(float dT);

    void updateProjectiles(float dT);

    void updateSpawnUnitsIfRequired(SDL_Renderer *renderer, float dT);

    void draw(SDL_Renderer *renderer);

    void addUnit(SDL_Renderer *renderer, Vector2D posMouse,int UnitType);

    void addTurret(SDL_Renderer *renderer, Vector2D posMouse, int TurretType);

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
    int wave;
    int target_hp;
    int money;
    TTF_Font *font;
    SDL_Surface *surfHPT ;
    SDL_Surface *surfMoneyT;
    SDL_Surface *surfWave;
    SDL_Surface *surfM;
    SDL_Texture *textHP;
    SDL_Texture *textMoney;
    SDL_Texture *textMode;
    SDL_Texture *textWave;
    SDL_Texture *defeatTexture;
    SDL_Texture *overlayTexture;
    bool overlayVisible = true;
};
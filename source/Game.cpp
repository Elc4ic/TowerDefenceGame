#include "Game.h"
#include "SDL2/SDL.h"
#include "TowerTypes.h"
#include "UnitTypes.h"
#include "Level.h"
#include "SDL2/SDL_ttf.h"


Game::Game(SDL_Window *window, SDL_Renderer *renderer, int windowWidth, int windowHeight) :
        placementModeCurrent(PlacementMode::archer),
        level(renderer, windowWidth / tileSize, windowHeight / tileSize),
        spawnTimer(0.3f), roundTimer(2.0f) {

    if (window != nullptr && renderer != nullptr) {
        textureCoin = TextureLoader::loadTexture(renderer, "coin.bmp");
        textureHP = TextureLoader::loadTexture(renderer, "HP.bmp");

        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        const float dT = 1.0f / 60.0f;
        money = 400;
        target_hp = 20;
        wave = 0;
        font = TTF_OpenFont("../Data/font/fox5.ttf", 24);


        bool running = true;
        while (running) {
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            if (timeDeltaFloat >= dT) {
                time1 = time2;

                processEvents(renderer, running);
                update(renderer, dT, running);
                draw(renderer);
            }
        }
        TTF_CloseFont(font);
    }
}


Game::~Game() {
    TextureLoader::deallocateTextures();
}


void Game::processEvents(SDL_Renderer *renderer, bool &running) {
    bool mouseDownThisFrame = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseDownThisFrame = (mouseDownStatus == 0);
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseDownStatus = SDL_BUTTON_LEFT;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    mouseDownStatus = SDL_BUTTON_RIGHT;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseDownStatus = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        running = false;
                        break;
                    case SDL_SCANCODE_1:
                        placementModeCurrent = PlacementMode::wizard;
                        break;
                    case SDL_SCANCODE_2:
                        placementModeCurrent = PlacementMode::archer;
                        break;
                    case SDL_SCANCODE_3:
                        placementModeCurrent = PlacementMode::grenadier;
                        break;
                    case SDL_SCANCODE_4:
                        placementModeCurrent = PlacementMode::froster
                                ;
                        break;


                }
        }
    }


    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    Vector2D posMouse((float) mouseX / tileSize, (float) mouseY / tileSize);

    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
            case SDL_BUTTON_LEFT:
                switch (placementModeCurrent) {
                    case PlacementMode::wizard:
                        if (mouseDownThisFrame && money >= wizardCost) {
                            addTurret(renderer, posMouse, 1);
                        }
                        break;
                    case PlacementMode::archer:
                        if (mouseDownThisFrame && money >= archerCost) {
                            addTurret(renderer, posMouse, 2);
                        }
                        break;
                    case PlacementMode::grenadier:
                        if (mouseDownThisFrame && money >= grenadierCost) {
                            addTurret(renderer, posMouse, 3);
                        }
                        break;
                    case PlacementMode::froster:
                        if (mouseDownThisFrame && money >= wizardCost) {
                            addTurret(renderer, posMouse, 4);
                        }
                        break;

                }
                break;


            case SDL_BUTTON_RIGHT:
                removeTurretsAtMousePosition(posMouse);
                break;
        }
    }
}


void Game::update(SDL_Renderer *renderer, float dT, bool &running) {
    updateUnits(dT);

    for (auto &turretSelected: listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);

    updateProjectiles(dT);

    updateSpawnUnitsIfRequired(renderer, dT);

    if (target_hp == 0) {
        listTurrets.clear();
        listUnits.clear();
        listProjectiles.clear();
        running = false;
    }
}


void Game::updateUnits(float dT) {
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(dT, level, listUnits, &target_hp);

            if (!(*it)->isAlive()) {
                money += 5 + wave;
                it = listUnits.erase(it);
                increment = false;
            }
        }

        if (increment)
            it++;
    }
}


void Game::updateProjectiles(float dT) {
    auto it = listProjectiles.begin();
    while (it != listProjectiles.end()) {
        (*it).update(dT, listUnits);

        if ((*it).getCollisionOccurred())
            it = listProjectiles.erase(it);
        else
            it++;
    }
}


void Game::updateSpawnUnitsIfRequired(SDL_Renderer *renderer, float dT) {
    spawnTimer.countDown(dT * (100 + 5 * wave) / 100);

    if (listUnits.empty() && spawnUnitCount == 0) {
        roundTimer.countDown(dT);
        if (roundTimer.timeSIsZero()) {
            wave++;
            spawnUnitCount = 10 + wave * wave;
            roundTimer.resetToMax();
        }
    }

    if (spawnUnitCount == 1 && wave % 5 == 0) {
        addUnit(renderer, level.getRandomEnemySpawnerLocation(), 4);
        spawnUnitCount--;
        spawnTimer.resetToMax();
    }

    if (spawnUnitCount > 0 && spawnTimer.timeSIsZero()) {
        if (spawnUnitCount % 20 == 0) {
            addUnit(renderer, level.getRandomEnemySpawnerLocation(), 3);
        } else if (spawnUnitCount % 7 == 0) {
            addUnit(renderer, level.getRandomEnemySpawnerLocation(), 2);
        } else addUnit(renderer, level.getRandomEnemySpawnerLocation(), 1);
        spawnUnitCount--;
        spawnTimer.resetToMax();
    }

}


void Game::draw(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);

    level.draw(renderer, tileSize);

    for (auto &unitSelected: listUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, tileSize, font);

    for (auto &turretSelected: listTurrets)
        turretSelected.draw(renderer, tileSize, font);

    for (auto &projectileSelected: listProjectiles)
        projectileSelected.draw(renderer, tileSize);

    int wHPT = 0, hHPT = 0;
    int wMT = 0, hMT = 0;
    int wHP = 0, hHP = 0;
    int wM = 0, hM = 0;
    int wC = 0, hC = 0;
    int wWave = 0, hWave = 0;

    SDL_Color TextColor = {255, 255, 255};
    surfHPT = TTF_RenderText_Blended(font, std::to_string(target_hp).c_str(), TextColor);
    surfMoneyT = TTF_RenderText_Blended(font, std::to_string(money).c_str(), TextColor);
    surfWave = TTF_RenderText_Blended(font, (std::to_string(wave) + "from 20 wave").c_str(), TextColor);
    switch (placementModeCurrent) {
        case PlacementMode::wizard:
            surfM = TTF_RenderText_Blended(font, ("wizard " + std::to_string(wizardCost)).c_str(), TextColor);
            break;
        case PlacementMode::archer:
            surfM = TTF_RenderText_Blended(font, ("archer " + std::to_string(archerCost)).c_str(), TextColor);
            break;
        case PlacementMode::grenadier:
            surfM = TTF_RenderText_Blended(font, ("grenadier " + std::to_string(grenadierCost)).c_str(), TextColor);
            break;
        case PlacementMode::froster:
            surfM = TTF_RenderText_Blended(font, ("froster " + std::to_string(wizardCost)).c_str(), TextColor);
            break;

    }
    textHP = SDL_CreateTextureFromSurface(renderer, surfHPT);
    textMoney = SDL_CreateTextureFromSurface(renderer, surfMoneyT);
    textMode = SDL_CreateTextureFromSurface(renderer, surfM);
    textWave = SDL_CreateTextureFromSurface(renderer, surfWave);

    SDL_QueryTexture(textHP, nullptr, nullptr, &wHPT, &hHPT);
    SDL_QueryTexture(textMoney, nullptr, nullptr, &wMT, &hMT);
    SDL_QueryTexture(textMode, nullptr, nullptr, &wM, &hM);
    SDL_QueryTexture(textWave, nullptr, nullptr, &wWave, &hWave);
    SDL_QueryTexture(textureHP, nullptr, nullptr, &wHP, &hHP);
    SDL_QueryTexture(textureCoin, nullptr, nullptr, &wC, &hC);

    SDL_Rect textHP_rect = {40, 5, wHPT, hHPT};
    SDL_Rect textM_rect = {110, 5, wMT, hMT};
    SDL_Rect textMode_rect = {250, 5, wM, hM};
    SDL_Rect textWave_rect = {800, 5, wWave, hWave};
    SDL_Rect rectHP = {5, 5, wHP, hHP};
    SDL_Rect rectC = {80, 5, wC, hC};

    SDL_RenderCopy(renderer, textHP, nullptr, &textHP_rect);
    SDL_RenderCopy(renderer, textMoney, nullptr, &textM_rect);
    SDL_RenderCopy(renderer, textMode, nullptr, &textMode_rect);
    SDL_RenderCopy(renderer, textWave, nullptr, &textWave_rect);
    SDL_RenderCopy(renderer, textureHP, nullptr, &rectHP);
    SDL_RenderCopy(renderer, textureCoin, nullptr, &rectC);

    SDL_FreeSurface(surfM);
    SDL_FreeSurface(surfWave);
    SDL_FreeSurface(surfMoneyT);
    SDL_FreeSurface(surfHPT);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(textHP);
    SDL_DestroyTexture(textMoney);
    SDL_DestroyTexture(textMode);
    SDL_DestroyTexture(textWave);
}


void Game::addUnit(SDL_Renderer *renderer, Vector2D posMouse, int UnitType) {
    if (UnitType == 1) {
        Solder solder = {renderer, posMouse, wave};
        listUnits.push_back(std::make_shared<Unit>(solder));
    } else if (UnitType == 2) {
        Runner runner = {renderer, posMouse, wave};
        listUnits.push_back(std::make_shared<Unit>(runner));
    } else if (UnitType == 3) {
        MiniBoss miniBoss = {renderer, posMouse, wave};
        listUnits.push_back(std::make_shared<Unit>(miniBoss));
    } else if (UnitType == 4) {
        Boss boss = {renderer, posMouse, wave};
        listUnits.push_back(std::make_shared<Unit>(boss));
    }
}


void Game::addTurret(SDL_Renderer *renderer, Vector2D posMouse, int TurretType) {
    for (auto it = listTurrets.begin(); it != listTurrets.end();) {
        if ((*it).checkIfOnTile((int) posMouse.x, (int) posMouse.y)) {
            (*it).lvlUp(&money);
            return;
        } else
            it++;
    }
    Vector2D pos((int) posMouse.x + 0.5f, (int) posMouse.y + 0.5f);
    if (TurretType == 1) {
        Wizard wizard = {renderer, pos};
        listTurrets.emplace_back(wizard);
        money -= wizardCost;
    } else if (TurretType == 2) {
        Archer arch = {renderer, pos};
        listTurrets.emplace_back(arch);
        money -= archerCost;
    } else if (TurretType == 4) {
        Froster frost = {renderer, pos};
        listTurrets.emplace_back(frost);
        money -= wizardCost;
    } else {
        Grenadier grenadier = {renderer, pos};
        listTurrets.emplace_back(grenadier);
        money -= grenadierCost;
    }
}

void Game::removeTurretsAtMousePosition(Vector2D posMouse) {
    for (auto it = listTurrets.begin(); it != listTurrets.end();) {
        if ((*it).checkIfOnTile((int) posMouse.x, (int) posMouse.y))
            it = listTurrets.erase(it);
        else
            it++;
    }
}
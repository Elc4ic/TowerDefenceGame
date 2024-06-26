#include "Game.h"
#include "SDL2/SDL.h"


Game::Game(SDL_Window *window, SDL_Renderer *renderer, int windowWidth, int windowHeight) :
        placementModeCurrent(PlacementMode::turret),
        level(renderer, windowWidth / tileSize, windowHeight / tileSize),
        spawnTimer(0.25f), roundTimer(5.0f) {

    if (window != nullptr && renderer != nullptr) {
        textureCoin = TextureLoader::loadTexture(renderer, "coin.bmp");
        textureHP = TextureLoader::loadTexture(renderer, "HP.bmp");

        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        const float dT = 1.0f / 60.0f;
        int money = 200;
        int target_hp = 20;


        bool running = true;
        while (running) {
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            if (timeDeltaFloat >= dT) {
                time1 = time2;

                processEvents(renderer, running, &money);
                update(renderer, dT, &target_hp, &money);
                draw(renderer);
            }
        }
    }
}


Game::~Game() {
    TextureLoader::deallocateTextures();
}


void Game::processEvents(SDL_Renderer *renderer, bool &running, int *money) {
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
                        placementModeCurrent = PlacementMode::wall;
                        break;
                    case SDL_SCANCODE_2:
                        placementModeCurrent = PlacementMode::turret;
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
                    case PlacementMode::wall:
                        /*   level.setTileWall((int) posMouse.x, (int) posMouse.y, true);*/
                        break;
                    case PlacementMode::turret:
                        if (mouseDownThisFrame && (*money) >= turrelCost) {
                            addTurret(renderer, posMouse);
                            (*money) -= turrelCost;
                            std::cout << (*money) << " ";
                        }
                        break;
                }
                break;


            case SDL_BUTTON_RIGHT:
                /*level.setTileWall((int) posMouse.x, (int) posMouse.y, false);*/
                removeTurretsAtMousePosition(posMouse);
                break;
        }
    }
}


void Game::update(SDL_Renderer *renderer, float dT, int *target_hp, int *money) {
    updateUnits(dT, target_hp, money);

    for (auto &turretSelected: listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);

    updateProjectiles(dT);

    updateSpawnUnitsIfRequired(renderer, dT);
}


void Game::updateUnits(float dT, int *target_hp, int *money) {
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(dT, level, listUnits, target_hp);

            if (!(*it)->isAlive()) {
                (*money) += 50;
                std::cout << (*money) << " ";
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
    spawnTimer.countDown(dT);

    if (listUnits.empty() && spawnUnitCount == 0) {
        roundTimer.countDown(dT);
        if (roundTimer.timeSIsZero()) {
            spawnUnitCount = 15;
            roundTimer.resetToMax();
        }
    }

    if (spawnUnitCount > 0 && spawnTimer.timeSIsZero()) {
        addUnit(renderer, level.getRandomEnemySpawnerLocation());

        spawnUnitCount--;
        spawnTimer.resetToMax();
    }
}


void Game::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    level.draw(renderer, tileSize);

    for (auto &unitSelected: listUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, tileSize);

    for (auto &turretSelected: listTurrets)
        turretSelected.draw(renderer, tileSize);

    for (auto &projectileSelected: listProjectiles)
        projectileSelected.draw(renderer, tileSize);

    /*   TTF_Font *Sans = TTF_OpenFont("Sans.ttf", 24);
       SDL_Color White = {255, 255, 255};
       SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans,"20/20", White);
       SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
       SDL_Rect Message_rect;
       Message_rect.x = 40;
       Message_rect.y = 5;
       Message_rect.w = 100;
       Message_rect.h = 30;*/

    int wHP = 0, hHP = 0;
    int wC = 0, hC = 0;
    SDL_QueryTexture(textureHP, nullptr, nullptr, &wHP, &hHP);
    SDL_QueryTexture(textureCoin, nullptr, nullptr, &wC, &hC);
    SDL_Rect rectHP = {5, 5, wHP, hHP};
    SDL_Rect rectC = {100, 5, wC, hC};
    SDL_RenderCopy(renderer, textureHP, nullptr, &rectHP);
    SDL_RenderCopy(renderer, textureCoin, nullptr, &rectC);

    SDL_RenderPresent(renderer);
}


void Game::addUnit(SDL_Renderer *renderer, Vector2D posMouse) {
    listUnits.push_back(std::make_shared<Unit>(renderer, posMouse));
}


void Game::addTurret(SDL_Renderer *renderer, Vector2D posMouse) {
    Vector2D pos((int) posMouse.x + 0.5f, (int) posMouse.y + 0.5f);
    listTurrets.emplace_back(renderer, pos);
}


void Game::removeTurretsAtMousePosition(Vector2D posMouse) {
    for (auto it = listTurrets.begin(); it != listTurrets.end();) {
        if ((*it).checkIfOnTile((int) posMouse.x, (int) posMouse.y))
            it = listTurrets.erase(it);
        else
            it++;
    }
}
#include "Level.h"


Level::Level(SDL_Renderer *renderer, int setTileCountX, int setTileCountY) :
        tileCountX(setTileCountX), tileCountY(setTileCountY) {
    textureTileWall = TextureLoader::loadTexture(renderer, "Tile Wall2.bmp");
    textureTileTarget = TextureLoader::loadTexture(renderer, "Tile Target.bmp");
    textureTileSpacer = TextureLoader::loadTexture(renderer, "Turret Spacer.bmp");
    textureTileEnemySpawner = TextureLoader::loadTexture(renderer, "EnemySpawner.bmp");
    textureTileEmpty = TextureLoader::loadTexture(renderer, "Tile Road.bmp");

    size_t listTilesSize = (size_t) tileCountX * tileCountY;
    listTiles.assign(listTilesSize, Tile{});

    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (map_creator[y][x] == 2) {
                setTileType(x, y, TileType::enemySpawner);
            } else if (map_creator[y][x] == 0) {
                setTileType(x, y, TileType::wall);
            } else if (map_creator[y][x] == 3) {
                targetX = x;
                targetY = y;
            } else if (map_creator[y][x] == 4) {
                setTileType(x, y, TileType::spacer);
            }
        }
    }

    calculateFlowField();
}


void Level::draw(SDL_Renderer *renderer, int tileSize) {

    for (int count = 0; count < listTiles.size(); count++)
        drawTile(renderer, (count % tileCountX), (count / tileCountX), tileSize);


    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (getTileType(x, y) == TileType::enemySpawner) {
                SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
                SDL_RenderCopy(renderer, textureTileEnemySpawner, nullptr, &rect);
            }
        }
    }

    if (textureTileTarget != nullptr) {
        SDL_Rect rect = {targetX * tileSize, targetY * tileSize, tileSize, tileSize};
        SDL_RenderCopy(renderer, textureTileTarget, nullptr, &rect);
    }

    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTileWall(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileWall, nullptr, nullptr, &w, &h);
                SDL_Rect rect = {
                        x * tileSize + tileSize / 2 - w / 2,
                        y * tileSize + tileSize / 2 - h / 2,
                        w,
                        h};
                SDL_RenderCopy(renderer, textureTileWall, nullptr, &rect);
            }
            if (isTileSpacer(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileSpacer, nullptr, nullptr, &w, &h);
                SDL_Rect rect = {
                        x * tileSize + tileSize / 2 - w / 2,
                        y * tileSize + tileSize / 2 - h / 2,
                        w,
                        h};
                SDL_RenderCopy(renderer, textureTileSpacer, nullptr, &rect);
            }
        }
    }
}

void Level::drawTile(SDL_Renderer *renderer, int x, int y, int tileSize) {
    SDL_Texture *textureSelected = textureTileEmpty;

    if (textureSelected != nullptr) {
        SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
        SDL_RenderCopy(renderer, textureSelected, nullptr, &rect);
    }
}


Vector2D Level::getRandomEnemySpawnerLocation() {
    std::vector<int> listSpawnerIndices;
    for (int count = 0; count < listTiles.size(); count++) {
        auto &tileSelected = listTiles[count];
        if (tileSelected.type == TileType::enemySpawner)
            listSpawnerIndices.push_back(count);
    }

    if (!listSpawnerIndices.empty()) {
        int index = listSpawnerIndices[rand() % listSpawnerIndices.size()];
        return Vector2D((float) (index % tileCountX) + 0.5f, (float) (index / tileCountX) + 0.5f);
    }

    return Vector2D(0.5f, 0.5f);
}


bool Level::isTileWall(int x, int y) {
    return (getTileType(x, y) == TileType::wall);
}

bool Level::isTileSpacer(int x, int y) {
    return (getTileType(x, y) == TileType::spacer);
}

Level::TileType Level::getTileType(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return listTiles[index].type;

    return TileType::empty;
}


void Level::setTileType(int x, int y, TileType tileType) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        listTiles[index].type = tileType;
        calculateFlowField();
    }
}


Vector2D Level::getTargetPos() {
    return Vector2D((float) targetX + 0.5f, (float) targetY + 0.5f);
}


void Level::calculateFlowField() {
    int indexTarget = targetX + targetY * tileCountX;
    if (indexTarget > -1 && indexTarget < listTiles.size() &&
        targetX > -1 && targetX < tileCountX &&
        targetY > -1 && targetY < tileCountY) {

        for (auto &tileSelected: listTiles) {
            tileSelected.flowDirectionX = 0;
            tileSelected.flowDirectionY = 0;
            tileSelected.flowDistance = flowDistanceMax;
        }

        calculateDistances();
        calculateFlowDirections();
    }
}


void Level::calculateDistances() {
    int indexTarget = targetX + targetY * tileCountX;

    std::queue<int> listIndicesToCheck;
    listTiles[indexTarget].flowDistance = 0;
    listIndicesToCheck.push(indexTarget);

    const int listNeighbors[][2] = {{-1, 0},
                                    {1,  0},
                                    {0,  -1},
                                    {0,  1}};

    while (!listIndicesToCheck.empty()) {
        int indexCurrent = listIndicesToCheck.front();
        listIndicesToCheck.pop();

        for (auto listNeighbor: listNeighbors) {
            int neighborX = listNeighbor[0] + indexCurrent % tileCountX;
            int neighborY = listNeighbor[1] + indexCurrent / tileCountX;
            int indexNeighbor = neighborX + neighborY * tileCountX;

            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                neighborX > -1 && neighborX < tileCountX &&
                neighborY > -1 && neighborY < tileCountY &&
                listTiles[indexNeighbor].type != TileType::wall &&
                listTiles[indexNeighbor].type != TileType::spacer) {

                if (listTiles[indexNeighbor].flowDistance == flowDistanceMax) {
                    listTiles[indexNeighbor].flowDistance = listTiles[indexCurrent].flowDistance + 1;
                    listIndicesToCheck.push(indexNeighbor);
                }
            }
        }
    }
}


void Level::calculateFlowDirections() {
    const int listNeighbors[][2] = {
            {-1, 0},
            {-1, 1},
            {0,  1},
            {1,  1},
            {1,  0},
            {1,  -1},
            {0,  -1},
            {-1, -1}};

    for (int indexCurrent = 0; indexCurrent < listTiles.size(); indexCurrent++) {
        if (listTiles[indexCurrent].flowDistance != flowDistanceMax) {
            unsigned char flowFieldBest = listTiles[indexCurrent].flowDistance;

            for (auto listNeighbor: listNeighbors) {
                int offsetX = listNeighbor[0];
                int offsetY = listNeighbor[1];

                int neighborX = offsetX + indexCurrent % tileCountX;
                int neighborY = offsetY + indexCurrent / tileCountX;
                int indexNeighbor = neighborX + neighborY * tileCountX;

                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < tileCountX &&
                    neighborY > -1 && neighborY < tileCountY) {
                    if (listTiles[indexNeighbor].flowDistance < flowFieldBest) {
                        flowFieldBest = listTiles[indexNeighbor].flowDistance;
                        listTiles[indexCurrent].flowDirectionX = offsetX;
                        listTiles[indexCurrent].flowDirectionY = offsetY;
                    }
                }
            }
        }
    }
}


Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float) listTiles[index].flowDirectionX, (float) listTiles[index].flowDirectionY).normalize();

    return Vector2D();
}
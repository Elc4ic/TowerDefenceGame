#pragma once

#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"


class Level {
private:
    enum class TileType : char {
        empty,
        wall,
        enemySpawner,
        spacer
    };

    static const unsigned char flowDistanceMax = 255;

    struct Tile {
        TileType type = TileType::empty;
        int flowDirectionX = 0;
        int flowDirectionY = 0;
        unsigned char flowDistance = flowDistanceMax;
    };


public:
    Level(SDL_Renderer *renderer, int setTileCountX, int setTileCountY);

    void draw(SDL_Renderer *renderer, int tileSize);

    Vector2D getRandomEnemySpawnerLocation();

    bool isTileWall(int x, int y);

    bool isTileSpacer(int x, int y);

    Vector2D getTargetPos();

    Vector2D getFlowNormal(int x, int y);

    const int map_creator[9][15] = {{0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},
                                    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                                    {0, 1, 4, 0, 4, 4, 1, 4, 4, 4, 4, 4, 1, 0, 0},
                                    {0, 1, 4, 2, 0, 4, 1, 1, 1, 3, 0, 4, 1, 0, 0},
                                    {0, 1, 0, 1, 0, 0, 4, 4, 4, 4, 0, 0, 2, 0, 0},
                                    {0, 1, 1, 1, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0},
                                    {0, 4, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };


private:
    TileType getTileType(int x, int y);

    void setTileType(int x, int y, TileType tileType);

    void drawTile(SDL_Renderer *renderer, int x, int y, int tileSize);

    void calculateFlowField();

    void calculateDistances();

    void calculateFlowDirections();


    std::vector<Tile> listTiles;
    int tileCountX, tileCountY;

    int targetX = 0, targetY = 0;

    SDL_Texture *textureTileWall = nullptr,
            *textureTileTarget = nullptr,
            *textureTileEnemySpawner = nullptr,
            *textureTileSpacer = nullptr,
            *textureTileEmpty = nullptr;
};
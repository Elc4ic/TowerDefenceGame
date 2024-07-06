#include "Turret.h"
#include "SDL2/SDL_ttf.h"

#include <cmath>

Turret::Turret(SDL_Renderer *renderer, Vector2D setPos, float timer, std::string texture, float range, float degSpeed,
               int damage, bool splash) :
        pos(setPos), angle(0.0f), timerWeapon(timer), weaponRange(range),
        speedAngular(MathAddon::angleDegToRad(degSpeed)), damage(damage), splash(splash), lvl(1) {
    textureMain = TextureLoader::loadTexture(renderer, texture);
    textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
}


void Turret::update(SDL_Renderer *renderer, float dT, std::vector<std::shared_ptr<Unit>> &listUnits,
                    std::vector<Projectile> &listProjectiles) {
    timerWeapon.countDown(dT);

    if (auto unitTargetSP = unitTarget.lock()) {
        if (!unitTargetSP->isAlive() ||
            (unitTargetSP->getPos() - pos).Vlenght() > weaponRange + lvl * 0.25) {
            unitTarget.reset();
        }
    }

    if (unitTarget.expired())
        unitTarget = findEnemyUnit(listUnits);

    if (updateAngle(dT))
        shootProjectile(renderer, listProjectiles);
}


bool Turret::updateAngle(float dT) {
    if (auto unitTargetSP = unitTarget.lock()) {
        Vector2D directionNormalTarget = (unitTargetSP->getPos() - pos).normalize();

        float angleToTarget = directionNormalTarget.angleBetween(Vector2D(angle));

        float angleMove = -std::copysign(speedAngular * dT, angleToTarget);
        if (abs(angleMove) > abs(angleToTarget)) {
            angle = directionNormalTarget.angle();
            return true;
        } else {
            angle += angleMove;
        }
    }

    return false;
}


void Turret::shootProjectile(SDL_Renderer *renderer, std::vector<Projectile> &listProjectiles) {
    if (timerWeapon.timeSIsZero()) {
        listProjectiles.emplace_back(renderer, pos, Vector2D(angle), damage + lvl, splash);

        timerWeapon.resetToMax();
    }
}


void Turret::draw(SDL_Renderer *renderer, int tileSize) {
    drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
    drawTextureWithOffset(renderer, textureMain, 0, tileSize);
    TTF_Font *font = TTF_OpenFont("../Data/font/fox5.ttf", 34);
    SDL_Color TextColor = {255, 255, 255};
    SDL_Surface *surfLVL = TTF_RenderText_Blended(font, (std::to_string(lvl) + " " +
                                                         std::to_string(50 * lvl * lvl)).c_str(), TextColor);
    SDL_Texture *textLVL = SDL_CreateTextureFromSurface(renderer, surfLVL);
    int wHP, hHP;
    SDL_QueryTexture(textLVL, nullptr, nullptr, &wHP, &hHP);
    SDL_Rect rectHP = {
            (int) (pos.x * tileSize),
            (int) (pos.y * tileSize) - hHP,
            wHP / 2,
            hHP / 2};

    SDL_RenderCopy(renderer, textLVL, nullptr, &rectHP);
}

void Turret::lvlUp(int *money) {
    int cost = 50 * lvl * lvl;
    if ((*money) >= cost) {
        (*money) -= cost;
        lvl += 1;
    }
}


void Turret::drawTextureWithOffset(SDL_Renderer *renderer, SDL_Texture *textureSelected,
                                   int offset, int tileSize) {
    if (renderer != nullptr && textureSelected != nullptr) {
        int w, h;
        SDL_QueryTexture(textureSelected, nullptr, nullptr, &w, &h);
        SDL_Rect rect = {
                (int) (pos.x * tileSize) - w / 2 + offset,
                (int) (pos.y * tileSize) - h / 2 + offset,
                w,
                h};
        SDL_RenderCopyEx(renderer, textureSelected, nullptr, &rect,
                         MathAddon::angleRadToDeg(angle), nullptr, SDL_FLIP_NONE);
    }
}


bool Turret::checkIfOnTile(int x, int y) {
    return ((int) pos.x == x && (int) pos.y == y);
}


std::weak_ptr<Unit> Turret::findEnemyUnit(std::vector<std::shared_ptr<Unit>> &listUnits) {
    std::weak_ptr<Unit> closestUnit;
    float distanceClosest = 0.0f;

    for (auto &unitSelected: listUnits) {
        if (unitSelected != nullptr) {
            float distanceCurrent = (pos - unitSelected->getPos()).Vlenght();
            if (distanceCurrent <= weaponRange &&
                (closestUnit.expired() || distanceCurrent < distanceClosest)) {
                closestUnit = unitSelected;
                distanceClosest = distanceCurrent;
            }
        }
    }

    return closestUnit;
}
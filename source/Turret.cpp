#include "Turret.h"

#include <cmath>


const float Turret::speedAngular = MathAddon::angleDegToRad(180.0f), Turret::weaponRange = 5.0f;


Turret::Turret(SDL_Renderer *renderer, Vector2D setPos) :
        pos(setPos), angle(0.0f), timerWeapon(1.0f) {
    textureMain = TextureLoader::loadTexture(renderer, "Turret.bmp");
    textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
}


void Turret::update(SDL_Renderer *renderer, float dT, std::vector<std::shared_ptr<Unit>> &listUnits,
                    std::vector<Projectile> &listProjectiles) {
    timerWeapon.countDown(dT);

    if (auto unitTargetSP = unitTarget.lock()) {
        if (!unitTargetSP->isAlive() ||
                (unitTargetSP->getPos() - pos).Vlenght() > weaponRange) {
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
        listProjectiles.emplace_back(renderer, pos, Vector2D(angle));

        timerWeapon.resetToMax();
    }
}


void Turret::draw(SDL_Renderer *renderer, int tileSize) {
    drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
    drawTextureWithOffset(renderer, textureMain, 0, tileSize);
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
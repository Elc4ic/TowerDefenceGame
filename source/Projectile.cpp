#include "Projectile.h"


const float Projectile::speed = 25.0f, Projectile::size = 0.2f;


Projectile::Projectile(SDL_Renderer *renderer, Vector2D setPos, Vector2D setDirectionNormal,
                       int damage, bool splash,float slow,float distMax) :
        pos(setPos), directionNormal(setDirectionNormal), damage(damage), splash(splash),slow(slow),distanceTraveledMax(distMax) {
    texture = TextureLoader::loadTexture(renderer, "Projectile.bmp");
}


void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>> &listUnits) {
    float distanceMove = speed * dT;
    pos += directionNormal * distanceMove;

    distanceTraveled += distanceMove;
    if (distanceTraveled >= distanceTraveledMax)
        collisionOccurred = true;

    checkCollisions(listUnits);
}


void Projectile::draw(SDL_Renderer *renderer, int tileSize) {
    if (renderer != nullptr) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect rect = {
                (int) (pos.x * tileSize) - w / 2,
                (int) (pos.y * tileSize) - h / 2,
                w,
                h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}


bool Projectile::getCollisionOccurred() {
    return collisionOccurred;
}


void Projectile::checkCollisions(std::vector<std::shared_ptr<Unit>> &listUnits) {
    if (!collisionOccurred) {
        for (int count = 0; count < listUnits.size() && !collisionOccurred; count++) {
            auto &unitSelected = listUnits[count];
            if (unitSelected != nullptr && unitSelected->checkOverlap(pos, size)) {
                if (splash) {
                    float splashSize = 1.5f;
                    for (int count = 0; count < listUnits.size() && !collisionOccurred; count++) {
                        auto &unitSelected = listUnits[count];
                        if (unitSelected != nullptr && unitSelected->checkOverlap(pos, splashSize)) {
                            unitSelected->removeHealth(damage,slow);
                        }
                    }
                } else {
                    unitSelected->removeHealth(damage,slow);
                }
                collisionOccurred = true;
            }
        }
    }
}
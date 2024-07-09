#include "Unit.h"

#include <cmath>
#include "Game.h"
#include "SDL2/SDL_ttf.h"


const float Unit::size = 0.5f;


Unit::Unit(SDL_Renderer *renderer, Vector2D setPos, const std::string &textureName, float speed, int hp) :
        pos(setPos), timerJustHurt(0.25f), hp(hp), speed(speed), timerSlowed(1.0f) {
    texture = TextureLoader::loadTexture(renderer, textureName);
}


void Unit::update(float dT, Level &level, std::vector<std::shared_ptr<Unit>> &listUnits, int *target_hp) {
    timerJustHurt.countDown(dT);
    timerSlowed.countDown(dT);

    float distanceToTarget = (level.getTargetPos() - pos).Vlenght();

    if (distanceToTarget < 0.5f) {
        (*target_hp)--;
        hp = 0;
    } else {
        float distanceMove;
        if (!timerSlowed.timeSIsZero()) distanceMove = speed * dT * slowed;
        else {
            distanceMove = speed * dT;
            slowed = 1.0f;
        }

        if (distanceMove > distanceToTarget)
            distanceMove = distanceToTarget;

        Vector2D directionNormal(level.getFlowNormal((int) pos.x, (int) pos.y));
        if ((int) pos.x == (int) level.getTargetPos().x && (int) pos.y == (int) level.getTargetPos().y)
            directionNormal = (level.getTargetPos() - pos).normalize();

        Vector2D posAdd = directionNormal * distanceMove;

        const float spacing = size / 2;
        int x = (int) (pos.x + posAdd.x + std::copysign(spacing, posAdd.x));
        int y = (int) (pos.y);
        if (posAdd.x != 0.0f && !level.isTileWall(x, y))
            pos.x += posAdd.x;
        x = (int) (pos.x);
        y = (int) (pos.y + posAdd.y + std::copysign(spacing, posAdd.y));
        if (posAdd.y != 0.0f && !level.isTileWall(x, y))
            pos.y += posAdd.y;
    }
}


void Unit::draw(SDL_Renderer *renderer, int tileSize, TTF_Font *font) {
    if (renderer != nullptr) {
        if (!timerSlowed.timeSIsZero()) {
            SDL_SetTextureColorMod(texture, 0, 0, 255);
        } else if (!timerJustHurt.timeSIsZero()) {
            SDL_SetTextureColorMod(texture, 255, 0, 0);
        } else
            SDL_SetTextureColorMod(texture, 255, 255, 255);

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect rect = {
                (int) (pos.x * tileSize) - w / 2,
                (int) (pos.y * tileSize) - h / 2,
                w,
                h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);

        SDL_Color TextColor = {255, 255, 255};
        surfHPT = TTF_RenderText_Blended(font, std::to_string(hp).c_str(), TextColor);
        textHP = SDL_CreateTextureFromSurface(renderer, surfHPT);
        int wHP, hHP;
        SDL_QueryTexture(textHP, nullptr, nullptr, &wHP, &hHP);
        SDL_Rect rectHP = {
                (int) (pos.x * tileSize) - wHP,
                (int) (pos.y * tileSize) - hHP,
                wHP / 2,
                hHP / 2};
        SDL_FreeSurface(surfHPT);
        SDL_RenderCopy(renderer, textHP, nullptr, &rectHP);
        SDL_DestroyTexture(textHP);
    }
}


bool Unit::checkOverlap(Vector2D posOther, float sizeOther) {
    return (posOther - pos).Vlenght() <= (sizeOther + size) / 2.0f;
}


bool Unit::isAlive() {
    return (hp > 0);
}


Vector2D Unit::getPos() {
    return pos;
}


void Unit::removeHealth(int damage, float slow) {
    if (damage > 0) {

        hp -= damage;
        if (hp < 0)
            hp = 0;

        if (slow != 0) {
            timerSlowed.resetToMax();
            slowed = slow;
        }

        timerJustHurt.resetToMax();
    }
}
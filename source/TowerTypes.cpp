#include "Turret.h"
#include "TowerTypes.h"

Wizard::Wizard(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.0f,
                "Wizard.bmp",
                5.0f, 180.0f, 2, false
        ) {

}

Grenadier::Grenadier(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                2.0f,
                "Grenadier.bmp",
                3.0f, 140.0f, 6, true
        ) {

}

Archer::Archer(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.4f,
                "Archer.bmp",
                8.0f,
                360.0f, 1, false
        ) {

}


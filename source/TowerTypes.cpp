#include "Turret.h"
#include "TowerTypes.h"

Wizard::Wizard(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.0f,
                "Wizard.bmp",
                5.0f, 200.0f, 6, false, 0, 20.0f,7
        ) {

}

Grenadier::Grenadier(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.5f,
                "Grenadier.bmp",
                3.0f, 140.0f, 6, true, 0, 20.0f,4
        ) {

}

Archer::Archer(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.3f,
                "Archer.bmp",
                6.0f,
                360.0f, 2, false, 0, 20.0f,3
        ) {

}

Froster::Froster(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.5f,
                "Frostier.bmp",
                1.7f,
                360.0f, 2, true, 0.60f, 1.5f,4
        ) {

}
#include "Turret.h"
#include "TowerTypes.h"

Wizard::Wizard(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.0f,
                "Wizard.bmp",
                5.0f, 180.0f, 6, false, 0, 20.0f,6
        ) {

}

Grenadier::Grenadier(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.5f,
                "Grenadier.bmp",
                3.0f, 140.0f, 6, true, 0, 20.0f,3
        ) {

}

Archer::Archer(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.3f,
                "Archer.bmp",
                6.0f,
                360.0f, 1, false, 0, 20.0f,2
        ) {

}

Froster::Froster(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.5f,
                "Frostier.bmp",
                1.5f,
                360.0f, 0, true, 0.5f, 1.5f,1
        ) {

}
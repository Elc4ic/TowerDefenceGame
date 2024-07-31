#include "Turret.h"
#include "TowerTypes.h"

Wizard::Wizard(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.0f,
                "Wizard.bmp",
                3.0f, 200.0f, 5, false, 1,0, 20.0f,5,10
        ) {
}

Grenadier::Grenadier(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                1.5f,
                "Grenadier.bmp",
                2.0f, 140.0f, 8, true,0, 0, 20.0f,12,0
        ) {
}

Archer::Archer(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.25f,
                "Archer.bmp",
                5.0f,
                360.0f, 4, false,0, 0, 20.0f,4,30
        ) {
}

Froster::Froster(SDL_Renderer *renderer, Vector2D setPos) :
        Turret::Turret(
                renderer,
                setPos,
                0.5f,
                "Frostier.bmp",
                1.7f,
                360.0f, 2, true,0, 0.60f, 1.5f,2,0
        ) {
}
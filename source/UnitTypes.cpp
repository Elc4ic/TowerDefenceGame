#include "Unit.h"
#include "UnitTypes.h"

Solder::Solder(SDL_Renderer *renderer, Vector2D setPos, int wave) :
        Unit(renderer, setPos, "Unit.bmp", 1.3f, 8 + wave * wave + wave) {

}

Runner::Runner(SDL_Renderer *renderer, Vector2D setPos, int wave) :
        Unit(renderer, setPos, "Runner.bmp", 3.2f + 0.07f * wave, 5 * wave) {

}

MiniBoss::MiniBoss(SDL_Renderer *renderer, Vector2D setPos, int wave) :
        Unit(renderer, setPos, "miniBoss.bmp", 1.1f, 10 + 4 * wave * wave) {

}

Boss::Boss(SDL_Renderer *renderer, Vector2D setPos, int wave) :
        Unit(renderer, setPos, "Boss.bmp", 1.1f, 70 + 12 * wave * wave) {

}
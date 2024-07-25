#include <iostream>
#include "SDL2/SDL.h"
#include "headers/Game.h"
#include "SDL2/SDL_ttf.h"


int main(int argc, char *args[]) {

    srand((unsigned) time(nullptr));

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        std::cout << "Error: Couldn't initialize SDL Video or Audio = " << SDL_GetError() << std::endl;
        std::cout << "Error: ttf = " << TTF_GetError() << std::endl;
        return 1;
    } else {

        SDL_Window *window = SDL_CreateWindow("Tower Base Defense",
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 990, 594, 0);
        if (window == nullptr) {
            std::cout << "Error: Couldn't create window = " << SDL_GetError() << std::endl;
            return 1;
        } else {
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                                    SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr) {
                std::cout << "Error: Couldn't create renderer = " << SDL_GetError() << std::endl;
                return 1;
            } else {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                SDL_RendererInfo rendererInfo;
                SDL_GetRendererInfo(renderer, &rendererInfo);
                std::cout << "Renderer = " << rendererInfo.name << std::endl;

                int windowWidth = 0, windowHeight = 0;
                SDL_GetWindowSize(window, &windowWidth, &windowHeight);

                Game game(window, renderer, windowWidth, windowHeight);

                SDL_DestroyRenderer(renderer);
            }

            SDL_DestroyWindow(window);
        }

        TTF_Quit();
        SDL_Quit();
    }
    return 0;
}
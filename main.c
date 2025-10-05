#include <SDL3/SDL.h>
#include <SDL3/SDL_test.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef WITH_SDLNET
#include <SDL3_net/SDL_net.h>
#endif

#define LOG_SDL_VERSION(WHAT, COMPILED_VERSION, LINKED_CBFN)                    \
    do {                                                                        \
        int compiled_version = COMPILED_VERSION;                                \
        int linked_version = LINKED_CBFN();                                     \
        LOG_CV_LV(WHAT, compiled_version, linked_version);                      \
    } while (0)

#define LOG_CV_LV(WHAT, CV, LV)                                                             \
    do {                                                                                    \
        SDL_Log(WHAT ": compiled version: %d.%d.%d, linked version: %d.%d.%d",              \
            SDL_VERSIONNUM_MAJOR(CV), SDL_VERSIONNUM_MINOR(CV), SDL_VERSIONNUM_MICRO(CV),   \
            SDL_VERSIONNUM_MAJOR(LV), SDL_VERSIONNUM_MINOR(LV), SDL_VERSIONNUM_MICRO(LV));  \
    } while (0)

int main(int argc, char *argv[]) {
    int i;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDLTest_TrackAllocations();

    LOG_SDL_VERSION("SDL", SDL_VERSION, SDL_GetVersion);
    LOG_SDL_VERSION("SDL_image", SDL_IMAGE_VERSION, IMG_Version);
    LOG_SDL_VERSION("SDL_mixer", SDL_MIXER_VERSION, MIX_Version);
    LOG_SDL_VERSION("SDL_ttf", SDL_TTF_VERSION, TTF_Version);
#ifdef WITH_SDLNET
    LOG_SDL_VERSION("SDL_net", SDL_NET_VERSION, NET_Version);
#endif

    if (argc != 1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: %s\n", argv[0]);
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }
    if (!SDL_CreateWindowAndRenderer("Hello", 640, 480, 0, &window, &renderer)) {
        return 1;
    }

    TTF_Init();
#ifdef WITH_SDLNET
    NET_Init();
#endif

    while (1) {
        int finished = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                finished = 1;
                break;
            }
        }
        if (finished) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
#ifdef WITH_SDLNET
    NET_Quit();
#endif
    TTF_Quit();
    SDL_Quit();

    SDLTest_LogAllocations();

    return 0;
}

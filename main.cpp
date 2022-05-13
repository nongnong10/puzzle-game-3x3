#include <iostream>
#include <SDL.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Quicker, Smarter";

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) // khoi tao SDL
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)     logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // SDL_CreateWindow(ten , toa do x cua cua so , toa do y cua cua so , do dai , do cao , loáº¡i);
    //mo cua so theo yeu cau da chon
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true); // kiem tra loi mo cua so


    //Moi truong binh thuong
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) //giai phong SDL
{
	SDL_DestroyRenderer(renderer); //giai phong bo nho quan ly but ve
	SDL_DestroyWindow(window); //giai phong bo nho quan ly cua so
	SDL_Quit();
}

int main(int argc, char* argv[])
{
    //test
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;

}

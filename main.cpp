#include<bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Quicker, Smarter";

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
SDL_Texture* texture=NULL;
TTF_Font* font=NULL;

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
//--------------------------------------------------------------------------------------
string      convert_to_String(int num){
    string res = "";
    while (num){
        res += num%10;
        num /= 10;
    }
    reverse(res.begin() , res.end());
    return res;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    //random 0->4
    int pic = 4;
    string path_img = string("img/test/picture") + convert_to_String(pic) + ".bmp";
    SDL_Surface* image = SDL_LoadBMP(path_img.c_str());
    texture = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);

    waitUntilKeyPressed();

    return 0;

}

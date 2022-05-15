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

void initSDL() // khoi tao SDL
{
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        logSDLError(std::cout , "SDL INIT" , true);
        exit(1);
    }

    //Create Window
    window = SDL_CreateWindow(WINDOW_TITLE.c_str() , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT , SDL_WINDOW_SHOWN);
    if (window == NULL){
        logSDLError(std::cout , "Create Window" , true);
        exit(1);
    }

    //Create Renderer
    renderer = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    //the renderer uses hardware acceleration
    //present is synchronized with the refresh rate
    SDL_SetRenderDrawColor(renderer , 0 , 0 , 0 , 0);

    SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);
    //?

    //Initialize the truetype font API.
    //This must be called before using other functions in this library, excepting TTF_WasInit.
    //SDL does not have to be initialized before this call.
    if (TTF_Init() == -1){
        cout<<TTF_GetError()<<endl;
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(imgFlags & IMG_Init(imgFlags))){
        logSDLError(std::cout , "Intialize SDL_Image" , true);
        exit(1);
    }
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))   return;
        SDL_Delay(100);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) //giai phong SDL
{
	TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(texture); texture=NULL;
    SDL_DestroyRenderer(renderer); renderer=NULL;
    SDL_DestroyWindow(window); window=NULL;
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

SDL_Texture*    loadTexture(std::string path){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL){
        logSDLError(std::cout , "Load Texture" , true);
    }
    else{
        newTexture = SDL_CreateTextureFromSurface(renderer , loadedSurface);
        if (newTexture == NULL){
            logSDLError(std::cout , "Create Texture From Surface" , true);
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

//--------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    srand(time(NULL));
    initSDL();

    //random 0->4
    int pic = 4;
    string path_img = string("img/test/picture") + convert_to_String(pic) + ".bmp";
    cout<<path_img;
    return 0;
    SDL_Surface* image = SDL_LoadBMP(path_img.c_str());
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    texture = loadTexture(path_img.c_str());
    //SDL_FreeSurface(image);

    bool quit = false;
    SDL_Event e;
    while (!quit){
        while (SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer , texture , NULL , NULL);
        SDL_RenderPresent(renderer);
    }

    return 0;

}

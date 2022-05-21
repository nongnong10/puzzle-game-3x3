#include<bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Solution.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Quicker, Smarter";

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
SDL_Texture* texture=NULL;
TTF_Font* font=NULL;

void    logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void    initSDL() // khoi tao SDL
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

void    waitUntilKeyPressed(){
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))   return;
        SDL_Delay(100);
    }
}

void    quitSDL() //giai phong SDL
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
        res += (char)(num%10 + '0');
        num /= 10;
    }
    reverse(res.begin() , res.end());
    return res;
}
//--------------------------------------------------------------------------------------

void    open(){
    freopen("aa.out","w",stdout);
}

int     chooseLevel(){

}

void    start_game(int level){
    BFS();
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    initSDL();

    //random 0->4
    int pic = 4;
    string path_img = string("img/picture") + convert_to_String(pic) + ".bmp"; /*0-5*/
    SDL_Surface *image = NULL;
    image = SDL_LoadBMP(path_img.c_str());
    if (image == NULL){
        logSDLError(std::cout , "Image" , true);
        return 0;
    }

    open();

    //choose level
    //int level = chooseLevel();
    int level = 1;

    //start game
    start_game(level);
    return 0;

    texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    waitUntilKeyPressed();
    quitSDL();
    return 0;

}


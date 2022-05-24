#include<bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Solution.h"
#include "Board.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int INF = 1e6+9;
const string WINDOW_TITLE = "Quicker, Smarter";

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;
SDL_Texture* texture=NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gWin = NULL;
Mix_Chunk *gLose = NULL;
Mix_Music *gMusic = NULL;
TTF_Font* font=NULL;

int     start, sol = 0;
int     N = 3;
int     cur_s, cur_m;

void    logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void    initSDL(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        logSDLError(std::cout , "SDL INIT" , true);
        exit(1);
    }

    window = SDL_CreateWindow(WINDOW_TITLE.c_str() , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT , SDL_WINDOW_SHOWN);
    if (window == NULL){
        logSDLError(std::cout , "Create Window" , true);
        exit(1);
    }

    renderer = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    //the renderer uses hardware acceleration
    //present is synchronized with the refresh rate
    SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);

    if (TTF_Init() == -1){
        cout<<TTF_GetError()<<endl;
        exit(1);
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
    Mix_FreeChunk( gMedium ); gMedium = NULL;
    SDL_DestroyTexture(texture); texture=NULL;
    SDL_DestroyRenderer(renderer); renderer=NULL;
    SDL_DestroyWindow(window); window=NULL;
    SDL_Quit();
    Mix_Quit();
}
//--------------------------------------------------------------------------------------
string      convert_to_String(int num){
    if (num <= 0)   return "0";
    string res = "";
    while (num){
        res += (char)(num%10 + '0');
        num /= 10;
    }
    reverse(res.begin() , res.end());
    return res;
}

void    write(const char *t, const SDL_Color &color, SDL_Rect *pos){
    SDL_Surface* message = TTF_RenderText_Solid(font , t , color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer , message);
    pos->w = message->w;
    pos->h = message->h;
    SDL_RenderCopy(renderer , text , NULL , pos);
    SDL_DestroyTexture(text);
    SDL_FreeSurface(message);
}

//--------------------------------------------------------------------------------------

SDL_Rect    grid(int pos){
    int x = (pos % N) * (SCREEN_WIDTH / N);
    int y = (pos / N) * (SCREEN_HEIGHT / N);
    return SDL_Rect{x , y , SCREEN_WIDTH/N , SCREEN_HEIGHT/N};
}

void    show_instruction(){
    SDL_Event e;

    while (SDL_WaitEvent(&e)){
        SDL_RenderClear(renderer);
        string t;
        SDL_Color   color = {98,22,254,255};
        SDL_Rect    pos;

        pos={185,100,0,0};
        t = "-> INSTRUCTION <-";
        write(t.c_str() , color , &pos);

        pos={130,150,0,0};
        t = "~ [S] to get solution";
        write(t.c_str() , color , &pos);

        pos={130,200,0,0};
        t = "~ [R] to reset";
        write(t.c_str() , color , &pos);

        pos={130,250,0,0};
        t = "~ [B] to see score board";
        write(t.c_str() , color , &pos);

        pos={130,300,0,0};
        t = "~ [Esc] to escape";
        write(t.c_str() , color , &pos);

        color={247,30,63,255};
        pos={115,400,0,0};
        t="Press [SPACE] to continue...";
        write(t.c_str(),color,&pos);

        SDL_SetRenderDrawColor(renderer,114,56,220,100);
        pos={140,80,360,280};
        SDL_RenderDrawRect(renderer,&pos);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);

        switch (e.type){
            case SDL_KEYDOWN:
                switch  (e.key.keysym.sym){
                    case SDLK_SPACE: return;
                    case SDLK_ESCAPE: exit(0);
                }
            break;
        }
    }
}

int     chooseLevel(){
    string t;
    SDL_Rect pos;
    SDL_Color color;

    SDL_RenderClear(renderer);
    t = "CHOOSE LEVEL FROM 1 TO 5";
    color={255,21,21,255};
    pos={120,200,0,0};
    write(t.c_str(),color,&pos);

    SDL_SetRenderDrawColor(renderer,200,50,50,100);
    pos = {110,190,400,53};
    SDL_RenderDrawRect(renderer,&pos);
    SDL_SetRenderDrawColor(renderer,0,0,0,250);
    SDL_RenderPresent(renderer);

    SDL_Event   event;
    int level = 0;
    while (SDL_WaitEvent(&event)){
        switch (event.type){
            case SDL_KEYDOWN:{
                switch (event.key.keysym.sym){
                    case SDLK_1: Mix_PlayChannel( -1, gMedium, 0 ), level = 1; break;
                    case SDLK_2: Mix_PlayChannel( -1, gMedium, 0 ), level = 2; break;
                    case SDLK_3: Mix_PlayChannel( -1, gMedium, 0 ), level = 3; break;
                    case SDLK_4: Mix_PlayChannel( -1, gMedium, 0 ), level = 4; break;
                    case SDLK_5: Mix_PlayChannel( -1, gMedium, 0 ), level = 5; break;
                }
                break;
            }
        }
        if (level)  break;
    }
    if (level == 0)     level = 1;

    SDL_RenderClear(renderer);
    t = "OKAY, LEVEL " + convert_to_String(level) + " IS LOADING...";
    pos = {120,200,0,0};
    write(t.c_str() , color , &pos);

    SDL_RenderPresent(renderer);

    return level;
}

void    show_board(Board B){
    SDL_RenderClear(renderer);
    SDL_Rect scrR , dstR;

    for (int i=0; i<N; ++i){
        for (int j=0; j<N; ++j){
            //blank
            if (B.A[i][j] == 1)     continue;

            scrR = grid(B.A[i][j] - 1);
            dstR = grid(i*N + j);
            SDL_RenderCopy(renderer , texture , &scrR , &dstR);
        }
    }

    SDL_RenderPresent(renderer);
}

void    gap_move(Board A, Board B, SDL_Rect moving){
    SDL_RenderClear(renderer);
    SDL_Rect srcR , dstR;
    for (int i=0; i<N; ++i){
        for (int j=0; j<N; ++j){
            srcR = grid(A.A[i][j] - 1);
            dstR = grid(i * N + j);
            if (A.A[i][j] == 1)     continue;
            if (B.A[i][j] == 1){
                dstR = moving;
            }
            SDL_RenderCopy(renderer , texture , &srcR , &dstR);
        }
    }
    SDL_RenderPresent(renderer);
}

void    moveBoard(Board &A , Board B){
    int speed = 20, delay = 4;
    SDL_Rect scrR = grid(B.blank);
    SDL_Rect dstR = grid(A.blank);

    //Move left or right
    while (abs(scrR.x - dstR.x) >= speed){
        int mov = (scrR.x - dstR.x) / abs(scrR.x - dstR.x);
        scrR.x -= speed * mov;
        gap_move(A , B , scrR);
        SDL_Delay(delay);
    }

    //Move up or down
    while (abs(scrR.y - dstR.y) >= speed){
        int mov = (scrR.y - dstR.y) / abs(scrR.y - dstR.y);
        scrR.y -= speed * mov;
        gap_move(A , B , scrR);
        SDL_Delay(delay);
    }

    A = B;
}

void    sboard(){
    SDL_Event event;
    while (SDL_WaitEvent(&event)){
        SDL_RenderClear(renderer);
        SDL_Color   color = {186,74,202,255};
        SDL_Rect    pos = {200,175,0,0};

        string  tim = "SCOREBOARD";
        write(tim.c_str() , color , &pos);

        //Top high score
        ifstream sbfile ("score.txt");
        color = {243,75,33,255};
        for (int i=1; i<=3; ++i){
            int m, s;
            sbfile >> m >> s;
            if (m > INF || s > INF) m = s = 0;
            pos.x = 165;
            pos.y += 50;
            tim = "TOP " + convert_to_String(i) + " : " + convert_to_String(m) + " min " + convert_to_String(s) + " sec.";
            write(tim.c_str() , color , &pos);
        }
        sbfile.close();
        SDL_RenderPresent(renderer);

        switch (event.type){
            case SDL_KEYDOWN:
                switch  (event.key.keysym.sym){
                    case SDLK_SPACE: return;
                    case SDLK_ESCAPE: exit(0);
                }
            break;
        }
    }
}

void    score_board(){
    SDL_Event event;
    while (SDL_WaitEvent(&event)){
        switch  (event.type){
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_b: return;
                }
            default:{
                SDL_RenderClear(renderer);
                int time = SDL_GetTicks() - start;
                int s = time / 1000;
                int m = s / 60;
                s = s % 60;

                //Current time
                SDL_Color   color = {222,237,39,255};
                SDL_Rect    pos = {120,100,0,0};
                string tim = "Current time: " + convert_to_String(m) + " min " + convert_to_String(s) + " sec.";
                write(tim.c_str() , color , &pos);

                //Scoreboard
                color = {186,74,202,255};
                pos = {200,175,0,0};
                tim = "SCOREBOARD";
                write(tim.c_str() , color , &pos);

                //Top high score
                ifstream sbfile ("score.txt");
                color = {243,75,33,255};
                for (int i=1; i<=3; ++i){
                    sbfile >> m >> s;
                    if (m > INF || s > INF) m = s = 0;
                    pos.x = 165;
                    pos.y += 50;
                    tim = "TOP " + convert_to_String(i) + " : " + convert_to_String(m) + " min " + convert_to_String(s) + " sec.";
                    write(tim.c_str() , color , &pos);
                }
                sbfile.close();
                SDL_RenderPresent(renderer);
                break;
            }
        }
    }
}

void    update_score(){
    int time = SDL_GetTicks() - start;
    cur_s = time / 1000;
    cur_m = cur_s / 60;
    cur_s %= 60;
    vector <pair<int,int>> V; V.clear();

    ifstream sbfile("score.txt");
    for (int i=0; i<3; ++i){
        int m, s;
        sbfile >> m >> s;
        V.push_back({m , s});
    }
    sbfile.close();

    V.push_back({cur_m , cur_s});
    ofstream _sbfile("score.txt");
    sort(V.begin() , V.end());
    for (int i=0; i<3; ++i){
        _sbfile << V[i].first << " " << V[i].second <<"\n";
    }
    _sbfile.close();
}

void    print_result(){
    if (sol == 0){
        Mix_PlayChannel( -1, gWin, 0 );
    }
    else{
        Mix_PlayChannel( -1, gLose, 0 );
    }
    SDL_Event   event;
    while (SDL_WaitEvent(&event)){
        SDL_RenderClear(renderer);
        SDL_Color  color = {245, 114, 15,255};
        SDL_Rect   pos = {170,175,0,0};
        string t;

        font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 36);
        if (sol == 0){
            pos = {110,175,0,0};
            t = "CONGRATULATION!";
            write(t.c_str() , color , &pos);

            //Result
            font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 24);
            pos = {125,225,0,0};
            color = {235,235,12,255};
            t = "YOUR RESULT : " + convert_to_String(cur_m) + " min " + convert_to_String(cur_s) + " sec.";
            write(t.c_str() , color , &pos);
        }
        else{
            pos = {190,175,0,0};
            t = "YOU LOSE! :<";
            write(t.c_str() , color , &pos);
            font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 24);
        }

        //Scoreboard
        pos = {200,300,0,0};
        color = {186,74,202,255};
        t = "SCOREBOARD";
        write(t.c_str() , color , &pos);

        ifstream sbfile ("score.txt");
        int m, s;
        color = {243,75,33,255};
        for (int i=1; i<=3; ++i){
            sbfile >> m >> s;
            if (m > INF || s > INF) m = s = 0;
            pos.x = 165;
            pos.y += 50;
            t = "TOP " + convert_to_String(i) + " : " + convert_to_String(m) + " min " + convert_to_String(s) + " sec.";
            write(t.c_str() , color , &pos);
        }
        sbfile.close();

        SDL_RenderPresent(renderer);

        switch (event.type){
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_SPACE: exit(0);
                    case SDLK_ESCAPE: exit(0);
                }
            break;
        }
    }
}

void    intro(){
    //Name game
    font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 60);
    SDL_RenderClear(renderer);
    string t = "Quicker";
    SDL_Rect pos = {120 , 100 , 0 , 0};
    SDL_Color color = {242, 164, 7 , 255};
    write(t.c_str() , color , &pos);

    font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 24);
    t = "and";
    pos = {150 , 180 , 0 , 0};
    color = {242, 164, 7 , 255};
    write(t.c_str() , color , &pos);

    font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 60);
    t = "Quicker";
    pos = {210 , 180 , 0 , 0};
    color = {242, 164, 7 , 255};
    write(t.c_str() , color , &pos);

    //Start
    font = TTF_OpenFont("WickedMouse-aGoK.ttf" , 24);
    t = "START";
    pos = {250 , 350 , 0 , 0};
    color = {242, 66, 7 , 255};
    write(t.c_str() , color , &pos);

    //instruction
    t = "instruction";
    pos = {205 , 400 , 0 , 0};
    write(t.c_str() , color , &pos);
    SDL_RenderPresent(renderer);

    //Scoreboard
    t = "Score board";
    pos = {200 , 450 , 0 , 0};
    write(t.c_str() , color , &pos);
    SDL_RenderPresent(renderer);

    //Quit
    t = "Quit";
    pos = {260 , 500 , 0 , 0};
    write(t.c_str() , color , &pos);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_WaitEvent(&event)){
        switch (event.type){
            case SDL_KEYDOWN:{
                switch (event.key.keysym.sym){
                    case SDLK_SPACE: return;
                    case SDLK_ESCAPE: exit(0);
                    case SDLK_b:{
                        Mix_PlayChannel( -1, gMedium, 0 );
                        sboard();
                        return;
                    }
                    case SDLK_i:{
                        Mix_PlayChannel( -1, gMedium, 0 );
                        show_instruction();
                        return;
                    }
                }
                break;
            }
        }
    }
}

void    start_game(int level){
    BFS();
    //instruction
    //show_instruction();

    //Render final image in 4 seconds
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer , texture , NULL , NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    //Timing
    start = SDL_GetTicks();

    //Make board
    Board B = Board(rd_level(level));
    show_board(B);

    //Run game
    SDL_Event   event;

    while (SDL_WaitEvent(&event)){
        switch (event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_UP:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            moveBoard(B,B.moveDown());
                            break;
                        }
                        case SDLK_DOWN:{
                            moveBoard(B,B.moveUp());
                            Mix_PlayChannel( -1, gMedium, 0 );
                            break;
                        }
                        case SDLK_LEFT:{
                            moveBoard(B,B.moveRight());
                            Mix_PlayChannel( -1, gMedium, 0 );
                            break;
                        }
                        case SDLK_RIGHT:{
                            moveBoard(B,B.moveLeft());
                            Mix_PlayChannel( -1, gMedium, 0 );
                            break;
                        }
                        case SDLK_s:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            while (B.getID() != 123456789){
                                int direction = trace(B.getID());
                                Mix_PlayChannel( -1, gMedium, 0 );
                                if (direction == 1)     moveBoard(B , B.moveDown());
                                if (direction == 2)     moveBoard(B , B.moveUp());
                                if (direction == 3)     moveBoard(B , B.moveRight());
                                if (direction == 4)     moveBoard(B , B.moveLeft());
                                SDL_Delay(40);
                            }
                            sol = 1;
                            break;
                        }
                        case SDLK_ESCAPE:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            return;
                        }
                        case SDLK_r:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            B = Board(rd_level(level));
                            show_board(B);
                            start = SDL_GetTicks();
                            break;
                        }
                        case SDLK_b:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            score_board();
                            show_board(B);
                            break;
                        }
                        case SDLK_i:{
                            Mix_PlayChannel( -1, gMedium, 0 );
                            show_instruction();
                            show_board(B);
                            break;
                        }
                    }
                break;
            }
            if (B.getID() == 123456789){
                cerr<<"GAME OVER!!!"<<'\n';
                if (!sol) update_score();
                break;
            }
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    initSDL();

    //random 0->5
    int pic = random(0 , 5);
    string path_img = string("img/picture") + convert_to_String(pic) + ".bmp"; /*0-5*/
    SDL_Surface *image = SDL_LoadBMP(path_img.c_str());
    if (image == NULL){
        logSDLError(std::cout , "Image" , true);
        return 0;
    }
    gMedium = Mix_LoadWAV( "mixkit-game-ball-tap-2073.wav" );
    //gWin = Mix_LoadWAV( "mixkit-festive-melody-audio-2985.wav" );
    gWin = Mix_LoadWAV( "mixkit-game-level-completed-2059.wav" );
    gLose = Mix_LoadWAV( "mixkit-horror-lose-2028.wav" );
	gMusic = Mix_LoadMUS( "mixkit-feeling-happy-5.mp3" );
    texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);

    if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic( gMusic, -1 );
    }
    intro();
    //font
    //font = TTF_OpenFont("font.ttf",24);

    //choose level
    int level = chooseLevel();
    //start game
    start_game(level);

    Mix_HaltMusic();

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    print_result();

    waitUntilKeyPressed();

    quitSDL();
    return 0;

}
